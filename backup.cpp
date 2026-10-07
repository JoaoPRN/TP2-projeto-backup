#include "backup.hpp"

#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

static bool diretorio_existe(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) return false;
    return (info.st_mode & S_IFDIR) != 0;
}

static bool criar_diretorios_recursivos(const std::string& path) {
    if (path.empty()) return false;
    if (diretorio_existe(path)) return true;

    size_t pos = path.find_last_of('/');
    if (pos != std::string::npos) {
        std::string parent = path.substr(0, pos);
        if (!parent.empty() && !diretorio_existe(parent)) {
            if (!criar_diretorios_recursivos(parent)) return false;
        }
    }

    if (mkdir(path.c_str(), 0755) != 0) {
        if (errno == EEXIST) return diretorio_existe(path);
        return false;
    }
    return true;
}

static std::string nome_arquivo_path(const std::string& path) {
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos) return path;
    return path.substr(pos + 1);
}

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
    if (lista.empty()) return false;

    if (!diretorio_existe(destino)) {
        if (!criar_diretorios_recursivos(destino)) return false;
    }

    bool sucesso = true;
    for (auto& arquivo : lista) {
    std::string nome = nome_arquivo_path(arquivo);
    std::string destino_final = destino + "/" + nome;
        if (!copiar_arquivo(arquivo, destino_final)) {
            std::cerr << "Erro ao copiar: " << arquivo << std::endl;
            sucesso = false;
        }
    }
    return sucesso;
}