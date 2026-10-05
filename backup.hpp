#ifndef BACKUP_HPP
#define BACKUP_HPP

#include <string>
#include <vector>

std::vector<std::string> ler_arquivos_parm(const std::string& nome_arquivo);

bool copiar_arquivo(const std::string& origem, const std::string& destino);

#endif 