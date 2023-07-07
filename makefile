TARGET	= lss.exe
SOURCE	= lsutil.c

$(TARGET): $(SOURCE)
	gcc $(SOURCE) -o $(TARGET)