OUT = score
CC := clang
TARGET := x86_64-pc-windows-gnu
SRC = src/dllmain.c minhook/src/buffer.c minhook/src/hook.c minhook/src/trampoline.c minhook/src/hde/hde32.c minhook/src/hde/hde64.c
OBJ = ${addprefix ${TARGET}/,${SRC:.c=.o}}
CFLAGS = -std=c99 -Iminhook/include -Wall -Ofast -target ${TARGET} -DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=_WIN32_WINNT_WIN7
LDFLAGS = -shared -static -static-libgcc -s

all: options ${OUT}

.PHONY: options
options:
	@echo "FLAGS	= ${FLAGS}"
	@echo "LDFLAGS	= ${LDFLAGS}"
	@echo "CC	= ${CC}"

.PHONY: ${TARGET}
${TARGET}:
	@mkdir -p ${TARGET}/src
	@mkdir -p ${TARGET}/minhook/src/hde 

${TARGET}/%.o: %.c 
	@echo BUILD $@
	@${CC} -c ${CFLAGS} $< -o $@

.PHONY: ${OUT}
${OUT}: ${TARGET} ${OBJ}
	@cd src && clang-format -i *.h *.c -style=file
	@echo LINK $@
	@${CC} ${CFLAGS} -o ${TARGET}/$@.dll ${OBJ} ${LDFLAGS}

.PHONY: clean
clean:
	rm -rf ${TARGET}
