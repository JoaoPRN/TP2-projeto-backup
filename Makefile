# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O0
COVFLAGS = -fprofile-arcs -ftest-coverage
DBGFLAGS = -g
LDFLAGS = -lgcov

# Arquivos
TARGET = testa_backup
SRC = backup.cpp testa_backup.cpp catch_amalgamated.cpp
OBJ = $(SRC:.cpp=.o)
REPORT_DIR = reports

# ===============================================
# Regras principais
# ===============================================

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJ)
	@echo "🔧 Compilando $(TARGET)..."
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)
	@echo "✅ Compilação concluída!"

compile:
	$(CXX) $(CXXFLAGS) -c backup.cpp

test: $(TARGET)
	@echo "🧪 Executando testes..."
	./$(TARGET)
	@echo "✅ Testes finalizados com sucesso!"

debug:
	$(CXX) $(CXXFLAGS) $(DBGFLAGS) -c backup.cpp
	$(CXX) $(CXXFLAGS) $(DBGFLAGS) $(OBJ) -o $(TARGET)
	gdb $(TARGET)

# ===============================================
# Cobertura de código (gcov)
# ===============================================
gcov:
	@echo "📊 Gerando cobertura de código..."
	$(CXX) $(CXXFLAGS) $(COVFLAGS) -c backup.cpp
	$(CXX) $(CXXFLAGS) $(COVFLAGS) -c testa_backup.cpp
	$(CXX) $(CXXFLAGS) $(COVFLAGS) -c catch_amalgamated.cpp
	$(CXX) $(CXXFLAGS) $(COVFLAGS) *.o -o $(TARGET) $(LDFLAGS)
	./$(TARGET)
	gcov backup.cpp
	@mkdir -p $(REPORT_DIR)
	mv *.gc* $(REPORT_DIR)
	mv *.gcov $(REPORT_DIR) 2>/dev/null || true
	@echo "✅ Relatório de cobertura salvo em $(REPORT_DIR)/"

# ===============================================
# Análises automáticas
# ===============================================
cpplint:
	@echo "🔍 Verificando estilo com cpplint..."
	cpplint --exclude=catch_amalgamated.hpp *.cpp *.hpp > $(REPORT_DIR)/cpplint.txt || true
	@echo "✅ Relatório salvo em $(REPORT_DIR)/cpplint.txt"

cppcheck:
	@echo "🔍 Verificando código com cppcheck..."
	cppcheck --enable=warning . > $(REPORT_DIR)/cppcheck.txt || true
	@echo "✅ Relatório salvo em $(REPORT_DIR)/cppcheck.txt"

valgrind: $(TARGET)
	@echo "🧠 Verificando memória com Valgrind..."
	valgrind --leak-check=yes --log-file=$(REPORT_DIR)/valgrind.txt ./$(TARGET)
	@echo "✅ Relatório salvo em $(REPORT_DIR)/valgrind.txt"

# ===============================================
# Geração de relatórios completos
# ===============================================
report: clean all gcov cppcheck cpplint valgrind
	@echo "📦 Todos os relatórios foram gerados em $(REPORT_DIR)/"
	@ls $(REPORT_DIR)

# ===============================================
# Limpeza
# ===============================================
clean:
	rm -rf *.o *.gc* $(TARGET) $(REPORT_DIR)
	@echo "🧹 Diretório limpo!"