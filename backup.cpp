#include "backup.hpp"

#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <cstdio>

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
    std::ifstream in(origem, std::ios::binary);
    if (!in.is_open()) {
        return false;
    }

    std::ofstream out(destino, std::ios::binary);
    if (!out.is_open()) {
        return false;
    }

    out << in.rdbuf();

    return in.good() && out.good();
}

bool realizar_backup(const std::vector<std::string>& lista, const std::string& destino) {
    return true;
}