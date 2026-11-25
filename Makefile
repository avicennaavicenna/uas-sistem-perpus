# Makefile - Sistem Manajemen Perpustakaan Berbasis CLI
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2

# Deteksi sistem operasi (Windows vs Linux/macOS)
ifeq ($(OS),Windows_NT)
    SHELL  = cmd.exe
    TARGET = perpustakaan.exe
    RUN    = .\$(TARGET)
    RM     = del /F /Q
    NULL   = 2>nul
else
    TARGET = perpustakaan
    RUN    = ./$(TARGET)
    RM     = rm -f
    NULL   =
endif

# Daftar berkas sumber dan objek
SRCS = main.c database.c catalog.c admin_menu.c circulation.c sorting.c
OBJS = $(SRCS:.c=.o)

.PHONY: all run clean reset-db

all: $(TARGET)

# Tahap Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Tahap Kompilasi per modul
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dependensi Header (otomatis recompile jika file .h diubah)
main.o:        main.c data_structure.h database.h admin_menu.h circulation.h
database.o:    database.c database.h data_structure.h
catalog.o:     catalog.c catalog.h database.h data_structure.h
admin_menu.o:  admin_menu.c admin_menu.h catalog.h database.h data_structure.h
circulation.o: circulation.c circulation.h database.h data_structure.h
sorting.o:     sorting.c sorting.h data_structure.h

# Jalankan program
run: $(TARGET)
	$(RUN)

# Hapus hasil kompilasi (.o dan executable)
clean:
	-$(RM) $(OBJS) $(TARGET) $(NULL)

# Hapus database operasional akun dan transaksi untuk reset cepat saat pengujian/demo
reset-db:
	-$(RM) user.txt peminjaman.txt $(NULL)
