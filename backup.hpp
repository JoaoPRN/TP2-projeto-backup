#ifndef BACKUP_HPP
#define BACKUP_HPP

#include <string>
#include <stdexcept>

/***************************************************************************
* Classe: BackupSystem
* Descrição:
*   Sistema de backup mínimo para suportar testes iniciais.
* Assertivas de entrada gerais:
*   - O nome do arquivo de parâmetros não deve ser vazio.
* Assertivas de saída gerais:
*   - Lança std::runtime_error se arquivo de parâmetros inexistente.
***************************************************************************/

class BackupSystem {
private:
   std::string arquivoParm;
   bool backupFlag; // true = HD -> Pen, false = Pen -> HD

public:
   explicit BackupSystem(const std::string& nomeArquivo);
   void setBackupFlag(bool flag);
   bool needCopy(const std::string& nomeArq);
   void restore();
   void backup();
   void processarInconsistencia();
};

#endif // BACKUP_HPP