CC=gcc
CDEFFLAGS=-std=c17 -Wall -Wextra -Wpedantic -Wconversion
CFLAGS=-O3 -Wl,--strip-all,--build-id=none,--gc-sections -fno-ident -D NDEBUG
CDEBFLAGS=-g -O0 -D _DEBUG

OBJ=obj
DOBJ=objd
SRC=src

TARGET=sloc

default: debug


$(OBJ):
	mkdir $(OBJ)
$(DOBJ):
	mkdir $(DOBJ)

$(OBJ)/%.c.o: $(SRC)/%.c
	$(CC) $(CDEFFLAGS) -c $^ -o $@ $(CFLAGS)

$(DOBJ)/%.c.o: $(SRC)/%.c
	$(CC) $(CDEFFLAGS) -c $^ -o $@ $(CDEBFLAGS)

srcs = $(wildcard $(SRC)/*.c)
srcs := $(subst $(SRC)/,,$(srcs))
objs_d = $(srcs:%=$(DOBJ)/%.o)
objs_r = $(srcs:%=$(OBJ)/%.o)

debug_obj: $(objs_d)
	$(CC) $^ -o deb$(TARGET).exe $(CDEBFLAGS) $(LIB)

release_obj: $(objs_r)
	$(CC) $^ -o $(TARGET).exe $(CFLAGS) $(LIB)

debug: $(DOBJ) debug_obj
release: $(OBJ) release_obj


clean.o:
	IF EXIST $(OBJ) rd /s /q $(OBJ)
	IF EXIST $(DOBJ) rd /s /q $(DOBJ)

clean: clean.o
	del *.exe