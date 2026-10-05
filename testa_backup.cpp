#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "backup.hpp"
#include <fstream>

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