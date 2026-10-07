#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "backup.hpp"
#include <fstream>
#include <filesystem>

TEST_CASE("Primeiro teste - ambiente configurado", "[init]") {
    REQUIRE(1 == 1);
}

TEST_CASE("Função ler_arquivos_parm", "[ler_arquivos_parm]") {
    // Esperamos apenas compilar por enquanto
    SUCCEED("Função implementada");
}

TEST_CASE("Erro ao ler um arquivo inexistente", "[ler_arquivos_parm]") {
    REQUIRE_THROWS_AS(ler_arquivos_parm("nao_existe.parm"), std::runtime_error);
}

TEST_CASE("Erro quando arquivo de parâmetros vazio", "[ler_arquivos_parm]") {
    std::ofstream("Backup.parm").close();  // cria arquivo vazio
    REQUIRE_THROWS_AS(ler_arquivos_parm("Backup.parm"), std::runtime_error);
}

TEST_CASE("Ler Backup.parm com caminhos válidos", "[ler_arquivos_parm]") {
    // Cria o arquivo Backup.parm no diretório atual
    std::ofstream parm("./Backup.parm", std::ios::out);
    REQUIRE(parm.is_open()); // garante que abriu corretamente
    parm << "dados/arquivo1.txt\n";
    parm << "dados/arquivo2.txt\n";
    parm << "dados/arquivo3.txt\n";
    parm.close(); // fecha o arquivo

    // leitura
    auto lista = ler_arquivos_parm("Backup.parm");

    // Verificações
    REQUIRE(lista.size() == 3);
    REQUIRE(lista[0] == "dados/arquivo1.txt");
    REQUIRE(lista[1] == "dados/arquivo2.txt");
    REQUIRE(lista[2] == "dados/arquivo3.txt");
}

TEST_CASE("Backup.parm com comentários e linhas vazias", "[ler_arquivos_parm]") {
    std::ofstream parm("Backup.parm");
    parm << "# Comentário\n";
    parm << "\n";
    parm << "dados/x.txt\n";
    parm.close();

    auto lista = ler_arquivos_parm("Backup.parm");
    REQUIRE(lista.size() == 1);
    REQUIRE(lista[0] == "dados/x.txt");
}

TEST_CASE("Falha ao tentar copiar arquivo inexistente", "[copiar_arquivo]") {
    bool resultado = copiar_arquivo("nao_existe.txt", "backup/nao_existe.txt");
    
    REQUIRE(resultado == false);
}

TEST_CASE("Cópia bem-sucedida de um arquivo existente", "[copiar_arquivo]") {
    std::ofstream arquivo_teste("dados/teste.txt");
    arquivo_teste << "12345";
    arquivo_teste.close();

    bool ok = copiar_arquivo("dados/teste.txt", "backup/teste.txt");
    REQUIRE(ok == true);

    std::ifstream f("backup/teste.txt");
    REQUIRE(f.good());
}

TEST_CASE("Conteúdo copiado corretamente", "[copiar_arquivo]") {
    std::ofstream("dados/original.txt") << "ABCDEF";

    copiar_arquivo("dados/original.txt", "backup/original.txt");

    std::ifstream original("dados/original.txt"), copia("backup/original.txt");
    std::string conteudo1((std::istreambuf_iterator<char>(original)), {});
    std::string conteudo2((std::istreambuf_iterator<char>(copia)), {});
    REQUIRE(conteudo1 == conteudo2);
}

TEST_CASE("Backup com lista de arquivos válida", "[realizar_backup]") {
    // prepara ambiente de teste
    system("mkdir -p dados backup");
    std::ofstream("dados/a.txt") << "A";
    std::ofstream("dados/b.txt") << "B";

    std::vector<std::string> lista = {"dados/a.txt", "dados/b.txt"};
    bool resultado = realizar_backup(lista, "backup");
    REQUIRE(resultado == true);

    // limpeza
    system("rm -f dados/a.txt dados/b.txt backup/a.txt backup/b.txt");
}


TEST_CASE("Backup falha se algum arquivo não existir", "[realizar_backup]") {
    std::vector<std::string> lista = {"dados/nao_existe.txt"};
    bool ok = realizar_backup(lista, "backup");
    REQUIRE(ok == false);
}

TEST_CASE("Integração completa: leitura do arquivo .parm e backup total", "[integracao]") {
    // Cria o arquivo de parâmetros
    std::ofstream parm("Backup.parm");
    parm << "dados/a.txt\n";
    parm << "dados/b.txt\n";
    parm.close();

    // Cria os arquivos originais
    std::ofstream("dados/a.txt") << "AAA";
    std::ofstream("dados/b.txt") << "BBB";

    auto lista = ler_arquivos_parm("Backup.parm");
    bool ok = realizar_backup(lista, "backup");

    REQUIRE(ok == true);
    std::ifstream fa("backup/a.txt");
    std::ifstream fb("backup/b.txt");
    REQUIRE(fa.good());
    REQUIRE(fb.good());
}

TEST_CASE("realizar_backup retorna falso para lista vazia", "[realizar_backup]") {
    std::vector<std::string> lista;
    bool resultado = realizar_backup(lista, "backup");
    REQUIRE(resultado == false);
}

TEST_CASE("realizar_backup cria diretório de destino se não existir", "[realizar_backup]") {
    std::filesystem::remove_all("backup_auto");
    std::vector<std::string> lista = {"dados/a.txt"};
    std::ofstream("dados/a.txt") << "teste";
    bool ok = realizar_backup(lista, "backup_auto");
    REQUIRE(ok == true);
    REQUIRE(std::filesystem::exists("backup_auto/a.txt"));
}