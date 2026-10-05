#include "backup.hpp"

#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

std::vector<std::string> ler_arquivos_parm(const std::string& nome_arquivo) {
    std::ifstream entrada(nome_arquivo);
    if (!entrada.is_open()) {
        throw std::runtime_error("Erro ao abrir arquivo de parâmetros");
    }
    std::vector<std::string> lista;
    std::string linha;
    while (getline(entrada, linha)) {
        if (linha.empty()) continue;
        if (linha[0] == '#') continue;
        lista.push_back(linha);
    }

    if (lista.empty()) {
        throw std::runtime_error("Arquivo de parâmetros vazio");
    }

    return lista;
}

bool copiar_arquivo(const std::string& origem, const std::string& destino) {
    if (!fs::exists(origem)) {
        return false;
    }
    return true;
}