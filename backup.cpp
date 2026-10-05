#include "backup.hpp"

#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>

std::vector<std::string> ler_arquivos_parm(const std::string& nome_arquivo) {
    std::ifstream entrada(nome_arquivo);
    if (!entrada.is_open()) {
        throw std::runtime_error("Erro ao abrir arquivo de parâmetros");
    }
    return {};
}