#include "backup.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/stat.h> // para obter mtime()

/***************************************************************************
* Função: BackupSystem
* Descrição:
*   Constrói o sistema de backup, verificando a existência do arquivo .parm.
* Parâmetros:
*   nomeArquivo - nome do arquivo de parâmetros.
* Assertivas de entrada:
*   nomeArquivo != "".
* Assertivas de saída:
*   Lança std::runtime_error se o arquivo não existir.
***************************************************************************/

BackupSystem::BackupSystem(const std::string& nomeArquivo)
    : arquivoParm(nomeArquivo), backupFlag(true) {
    if (nomeArquivo.empty()) {
        throw std::runtime_error("Nome do arquivo vazio");
    }
    std::ifstream f(arquivoParm);
    if (!f.good()) {
        throw std::runtime_error("Arquivo Backup.parm inexistente");
    }
}

/***************************************************************************
* Função: setBackupFlag
* Descrição:
*   Define o modo de operação (backup ou restauração).
* Parâmetros:
*   flag - true para backup (HD→Pen), false para restauração (Pen→HD).
* Assertivas de entrada:
*   Nenhuma.
* Assertivas de saída:
*   backupFlag é atualizado corretamente.
***************************************************************************/

void BackupSystem::setBackupFlag(bool flag) {
    backupFlag = flag;
}

/***************************************************************************
* Função auxiliar: filesEqual
* Descrição:
*   Compara o conteúdo de dois arquivos binários.
* Parâmetros:
*   a, b - caminhos dos arquivos a comparar.
* Valor retornado:
*   true se os arquivos forem idênticos, false caso contrário.
***************************************************************************/

static bool filesEqual(const std::string& a, const std::string& b) {
    std::ifstream fa(a, std::ios::binary);
    std::ifstream fb(b, std::ios::binary);
    if (!fa || !fb) return false; // se algum não existe

    std::string sa((std::istreambuf_iterator<char>(fa)), {});
    std::string sb((std::istreambuf_iterator<char>(fb)), {});
    return sa == sb;
}

/***************************************************************************
* Função auxiliar: mtime
* Descrição:
*   Retorna a data/hora da última modificação de um arquivo.
* Parâmetros:
*   p - caminho do arquivo.
* Valor retornado:
*   time_t com o tempo da última modificação (0 se não existir).
***************************************************************************/

static time_t mtime(const std::string& p) {
    struct stat st;
    if (stat(p.c_str(), &st) != 0) return 0;
    return st.st_mtime;
}

/***************************************************************************
* Função: needCopy
* Descrição:
*   Verifica se o arquivo no HD e no Pen-drive devem ser copiados, levando
*   em conta existência e datas de modificação.
* Parâmetros:
*   nomeArq - nome do arquivo sem prefixo.
* Valor retornado:
*   true se precisa copiar, false caso contrário.
* Assertivas de entrada:
*   nomeArq != "".
* Assertivas de saída:
*   Retorna true se o HD for mais novo que o Pen, ou se um arquivo não existir.
***************************************************************************/

bool BackupSystem::needCopy(const std::string& nomeArq) {
    std::string hd = "HD_" + nomeArq;
    std::string pen = "PEN_" + nomeArq;
    std::ifstream fha(hd), fpa(pen);

    // Caso 1: ambos ausentes -> nada a fazer
    if (!fha.good() && !fpa.good())
        return false;

    // Caso 2: Pen ausente -> precisa copiar HD → Pen
    if (!fpa.good())
        return true;

    // Caso 3: HD ausente -> precisa restaurar Pen → HD (tratado como cópia)
    if (!fha.good())
        return true;

    // Caso 4: ambos existem -> verifica data de modificação
    time_t th = mtime(hd);
    time_t tp = mtime(pen);

    if (th > tp)
        return true;  // HD mais novo → precisa copiar
    if (tp > th)
        return false; // Pen mais novo → não precisa copiar (restauração futura)

    // Caso 5: datas iguais → compara conteúdo
    return !filesEqual(hd, pen);
}

/***************************************************************************
* Função: restore
* Descrição:
*   Simula a restauração dos arquivos do Pen-drive para o HD.
* Assertivas de entrada:
*   backupFlag == false.
* Assertivas de saída:
*   Mensagem impressa indica a direção correta (Pen → HD).
***************************************************************************/

void BackupSystem::restore() {
    std::cout << "Restaurando Pen-drive -> HD\n";
}

/***************************************************************************
* Função: backup
* Descrição:
*   Simula a cópia de arquivos do HD para o Pen-drive.
* Assertivas de entrada:
*   backupFlag == true.
* Assertivas de saída:
*   Mensagem indica operação HD → Pen-drive.
***************************************************************************/

void BackupSystem::backup() {
    std::cout << "Backup HD -> Pen-drive\n";
}

/***************************************************************************
* Função: processarInconsistencia
* Descrição:
*   Lança exceção quando ocorre uma combinação ilegal
*   (situação “Impossível” na tabela de decisão).
* Assertivas de entrada:
*   Nenhuma.
* Assertivas de saída:
*   Lança std::logic_error com uma mensagem de idErro.
***************************************************************************/

void BackupSystem::processarInconsistencia() {
    throw std::logic_error("Combinação ilegal (idErro)");
}