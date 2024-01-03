# Компилятор
CC = gcc

# Опции компилятора
CFLAGS = -Wall -Iinclude -Wextra -pedantic -O0 -g

# Директории
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Список исходных файлов
SOURCES = $(wildcard $(SRCDIR)/*.c)

# Преобразование исходных файлов в объектные файлы
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Имя исходного файла
TARGET = $(BINDIR)/pthread

# Имя статической библиотеки
LIB_STATIC = $(BINDIR)/libpthread.a


all: $(BINDIR) $(LIB_STATIC)

# Цель для исполняемого файла
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Цель для объектных файлов
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Цель для статической библиотеки
$(LIB_STATIC): $(OBJECTS)
	ar rcs $@ $^

# Создание директории obj, если она не существует
$(OBJDIR):
	mkdir -p $(OBJDIR) 

# Создание директории obj, если она не существует
$(BINDIR):
	mkdir -p $(BINDIR) 

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
