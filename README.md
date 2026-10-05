# Instruções para Compilação e Versionamento

1. **Pré-requisitos**
   - g++ instalado
   - make instalado
   - Todos os arquivos devem estar no mesmo diretório

2. **Compilação**
   - Para compilar tudo, execute no terminal:
     ```
     make
     ```
   - Para executar uma regra específica, use:
     ```
     make <nome_da_regra>
     ```
     Exemplo:
     ```
     make cppcheck
     ```

3. **Controle de Versão**
   - Faça um commit no Git para cada teste ou código adicionado:
     ```
     git add .
     git commit -m "Descrição do que foi adicionado ou modificado"
     ```
