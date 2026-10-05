#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "backup.hpp"

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