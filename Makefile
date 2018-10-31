VAR=gcc

TARGET=myshell

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(VAR) $(TARGET).c -o $(TARGET)

clean :
	rm -rf $(TARGET)
