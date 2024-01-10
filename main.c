#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_IDENTIFIER_SIZE 30
#define MAX_TOKEN_LENGTH 30
#define MAX_VALUE_SIZE 100


typedef struct {
    char name[MAX_IDENTIFIER_SIZE];
    int type; // 0: text, 1: int
    char value[MAX_VALUE_SIZE];
} Variable;

int asText(const char* myString) { //asText komutu fonksiyonu

    char* endptr;
    int result = strtol(myString, &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Error: Input rakam disinda baska karakter iceriyor\n");
        exit(1); // Terminate the program or handle the error as desired
    }

    return atoi(myString);

}

char* asString(int myInt) { //asString komutu fonksiyonu

    static char str[20]; // Assuming a sufficiently large buffer to hold the string

    if (isalpha(myInt)) {
        snprintf(str, sizeof(str), "Error: Input rakam disinda baska karakter iceriyor\n");
    } else {
        snprintf(str, sizeof(str), "%d", myInt);
    }

    return str;
}


void override(char* myText, int location, const char* ovrText) { //override komutu fonksiyonu
    int myTextLen = strlen(myText);
    int ovrTextLen = strlen(ovrText);

    int i;
    for (i = 0; i < ovrTextLen + myTextLen; i++) {
        myText[location + i] = ovrText[i];
    }
}

void atamaIslemi(Variable* variables, const char* name1){ //ismi, variables dizisinden bulup value atar
    for (int k = 0; k < MAX_TOKEN_LENGTH; ++k) {
        if (strcmp(variables[k].name, name1) == 0) {
            strcpy(name1, variables[k].value);
            break;
        }
    }
}

void removeChars(char *s, int start, int end) { //karakter silme fonksiyonu
    int length = strlen(s);
    int i, j;

    if (start < 0 || end >= length || start > end)
        return;

    for (i = start, j = end + 1; s[j] != '\0'; i++, j++) {
        s[i] = s[j];
    }

    s[i] = '\0';
}

char* insert(const char* myText, int location, const char* insertText) { //insert komut fonksiyonu
    int myTextLen = strlen(myText);
    int insertTextLen = strlen(insertText);

    if (location < 0 || location > myTextLen)
        return NULL;

    // Hedef dizinin boyutunu hesapla
    int resultLen = myTextLen + insertTextLen;
    char* result = (char*)malloc((resultLen + 1) * sizeof(char));

    // myText'in başından location'a kadar olan kısmı kopyala
    strncpy(result, myText, location);
    result[location] = '\0';

    // insertText'i kopyala
    strcat(result, insertText);

    // myText'in location'dan sonraki kısmını kopyala
    strcat(result, myText + location);

    return result;
}


int locate(const char *bigText, const char *smallText, int start) { //locate komut fonskiyonu
    int bigTextLen = strlen(bigText);
    int smallTextLen = strlen(smallText);

    if (start < 0 || start >= bigTextLen)
        return 0;

    for (int i = start; i <= bigTextLen - smallTextLen; i++) {
        int j;
        for (j = 0; j < smallTextLen; j++) {
            if (bigText[i + j] != smallText[j])
                break;
        }
        if (j == smallTextLen)
            return i;
    }

    return 0;
}


void subs(char *myText, int begin, int end) { // subs komutu fonksiyonu !!! end index'i dahil değildir !!!
    int i, j;
    for (i = begin, j = 0; i < end; i++, j++) {
        myText[j] = myText[i];
    }
    myText[j] = '\0';
}

void string_cikar(char* str, const char* cikarilacak,char* strcopy) {
    int str_uzunluk = strlen(str);
    int cikarilacak_uzunluk = strlen(cikarilacak);

    strcpy(strcopy,str);

    int i, j, k;
    for (i = 0; i <= str_uzunluk - cikarilacak_uzunluk; i++) {
        j = 0;
        k = i;
        while (strcopy[k] == cikarilacak[j] && cikarilacak[j] != '\0') {
            k++;
            j++;
        }
        if (cikarilacak[j] == '\0') {
            while (strcopy[k] != '\0') {
                strcopy[i] = strcopy[k];
                i++;
                k++;
            }
            strcopy[i] = '\0';
            break;
        }
    }
}

bool isDigit(char* str) {
    char str2[strlen(str)];
    strcpy(str2,str);
    if(strstr((char*)str2,"+")){
        int len = strlen(str2);
        int i, j;

        for (i = 0, j = 0; i < len; i++) {
            if (str2[i] != '+') {
                str2[j] = str2[i];
                j++;
            }
        }

        str2[j] = '\0';
    }
    else if(strstr((char*)str2,"-")){
        int len = strlen(str2);
        int i, j;

        for (i = 0, j = 0; i < len; i++) {
            if (str2[i] != '-') {
                str2[j] = str2[i];
                j++;
            }
        }

        str2[j] = '\0';
    }
    while (*str != '\0') {
        if (!isdigit(*str2)) {
            return false;
        }
        str++;
    }

    return true;
}

void getInnerStringInParentheses(const char *text, char *innerString) {
    const char *start = strchr(text, '(');  // Find the opening parenthesis
    const char *end = strchr(text, ')');    // Find the closing parenthesis

    if (start != NULL && end != NULL && end > start) {  // Ensure both parentheses are found and in the correct order
        size_t innerStringLength = end - start - 1;  // Calculate the length of the inner string excluding the parentheses
        strncpy(innerString, start + 1, innerStringLength);  // Copy the inner string from the text (excluding the parentheses)
        innerString[innerStringLength] = '\0';  // Null-terminate the inner string
    } else {
        innerString[0] = '\0';  // If parentheses are not found or are in the incorrect order, set an empty inner string
    }
}

void removeTrailingSpaces(char* str) {
    int len = strlen(str);
    int semicolon_index = -1;

    for (int i = 0; i < len; i++) {
        if (str[i] == ';') {
            semicolon_index = i;
            int j = semicolon_index + 1;

            // Skip trailing spaces
            while (j < len && isspace(str[j])) {
                j++;
            }

            if (j > semicolon_index + 1) {
                // Shift characters to remove trailing spaces
                memmove(&str[semicolon_index + 1], &str[j], len - j + 1);
                len -= (j - semicolon_index - 1);
            }
        }
    }
}

void removeQuotes(char *str) { //tırnakları kaldırma
    int length = strlen(str);

    if (length >= 2 && str[0] == '"' && str[length - 1] == '"') {
        // Move the characters one position to the left, excluding the starting and ending quotes
        for (int i = 1; i < length - 1; i++) {
            str[i - 1] = str[i];
        }
        // Null-terminate the string after removing the quotes
        str[length - 2] = '\0';
    }
}

void* getVariableValue(Variable* variables, const char* variableName) {

    for (int i = 0; i < MAX_TOKEN_LENGTH; i++) {



        if(strcmp(variables[i].name, variableName) == 0){
            return variables[i].value;

        }
    }
    printf("Variable Value getirelemedi!*\n");
    return NULL;

}

void getKeyboardInput(char *promptText,char *keyboardInput) {

    printf("%s: ", promptText);
    fgets(keyboardInput, 100, stdin);
}


void removeNewLine(char *str) {
    size_t length = strlen(str);

    if (length > 0 && str[length - 1] == '\n') {
        str[length - 1] = '\0';
    }
}

const char* readFile(char *fileName) {
    FILE    *textfile;
    char    *string;
    long    numbytes;

    // fileName arkasina .txt ekliyoruz
    char txtString[4] = ".txt";
    strncat(fileName, txtString, strlen(fileName));

    textfile = fopen(fileName, "r");
    if(textfile == NULL)
        return "1";

    fseek(textfile, 0L, SEEK_END);
    numbytes = ftell(textfile);
    fseek(textfile, 0L, SEEK_SET);

    string = (char*)calloc(numbytes, sizeof(char));
    if(string == NULL)
        return "1";

    fread(string, sizeof(char), numbytes, textfile);
    fclose(textfile);

    return string;
}

void writeFile(char *string, char *fileName) {

    // fileName arkasina .txt ekliyoruz
    char txtString[4] = ".txt";
    strncat(fileName, txtString, strlen(fileName));

    // garip bir hata kaynakli digit tuttugumuz stringlerde .txt4 seklinde ekleme yapiyor
    // onu engellemek icin son char dizisinin son elemanı 4 mu diye bakiyoruz 4'se siliyoruz
    if (fileName[strlen(fileName) - 1] == '4') {
        fileName[strlen(fileName) - 1] = '\0';
    }

    FILE *fp;

    fp = fopen(fileName, "a");

    if (fp) {
        fputs(string, fp);
    }
    else {
        printf("Failed to open the file\n");
    }
    fclose(fp);
}

char* addQuotes(const char* input) {
    size_t inputLength = strlen(input);

    // Allocate memory for the output string
    size_t outputLength = inputLength + 2; // 2 extra characters for the quotes
    char* output = (char*)malloc((outputLength + 1) * sizeof(char)); // +1 for null terminator
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    // Copy the input string to the output string with quotes
    output[0] = '"';
    strncpy(output + 1, input, inputLength);
    output[outputLength - 1] = '"';
    output[outputLength] = '\0';

    return output;
}

void createVariable(Variable* variables,const char* variableName, int type) {
    // MAX_TOKEN_LENGTH boyunca döngü yapılıyor
    for (int i = 0; i < MAX_TOKEN_LENGTH; i++) {
        // Eğer variables dizisinin ilgili indeksindeki name boşsa, yeni değişken atanabilir
        if (strcmp(variables[i].name, "") == 0) {
            // Değişkenin adı kopyalanıyor
            strcpy(variables[i].name, variableName);
            // Değişkenin tipi atanıyor
            variables[i].type = type;

            // Değişken tipine göre value alanı ayrılıyor ve başlangıç değeri atanıyor
            if (type == 0) {  // Text tipi
                strcpy(variables[i].value, "");
            } else {  // Integer tipi
                strcpy(variables[i].value,"");
            }

            // İşlem tamamlandıktan sonra döngüden çıkılıyor
            break;
        }
    }
}

void assignVariable(Variable* variables, const char* name, int type, void* value) {
    // MAX_TOKEN_LENGTH boyunca döngü yapılıyor
    for (int i = 0; i < MAX_TOKEN_LENGTH; i++) {
        // Eğer variables dizisinin ilgili indeksindeki name boş değilse ve aranan değişken adına sahipse,
        // atama işlemi gerçekleştirilebilir
        if (variables[i].name[0] != '\0' && strcmp(variables[i].name, name) == 0) {
            // Değişkenin tipi kontrol ediliyor
            if (variables[i].type == type) {
                // Değişken tipine göre value alanına atama yapılıyor
                if (type == 0) {  // Text tipi
                    strcpy(variables[i].value, value);
                } else {  // Integer tipi
                    int intValue = *((int*)value);
                    *((int*)variables[i].value) = intValue;
                }

                // İşlem tamamlandıktan sonra döngüden çıkılıyor
                break;
            } else {
                printf("Hata: Geçersiz atama tipi!\n");
            }
        }
    }
}


//Komutları yorumla
void interpretStatement(Variable* variables, const char* statement) {
    /*  char lastChar = statement[strlen(statement) - 1];
      if(statement[lastChar] != ';'){
          printf("Sonda ; unuttunuz!\n");
      } */
    removeTrailingSpaces(statement);
    size_t length = strlen((char *)statement);
    char lastCharacter = statement[length - 1];

    if (lastCharacter == ';') {


            // read komutunu kontrol ediyor ve read kelimesinin string icersinde gecmedigini kontrol ediyor
        if (strstr(statement, "read") != NULL & strstr(statement, "\"") == NULL) {


            removeChars(statement, 0, 4); // read kelimesini siler
            removeChars(statement, strlen(statement) - 1, strlen(statement) - 1); // ';' karakterini siler


            char text[strlen(statement)];
            strcpy(text, statement);
            char *tokencpy;
            char name1[99];
            char name2[99];
            char name3[99];

            tokencpy = strtok(statement, " ");
            if (tokencpy != NULL) {
                strncpy(name1, tokencpy, sizeof(name1));
                name1[sizeof(name1) - 1] = '\0';
            }

            tokencpy = strtok(NULL, " ");
            if (tokencpy != NULL) {
                strncpy(name2, tokencpy, sizeof(name2));
                name2[sizeof(name2) - 1] = '\0';
            }

            tokencpy = strtok(NULL, " ");
            if (tokencpy != NULL) {
                strncpy(name3, tokencpy, sizeof(name3));
                name3[sizeof(name3) - 1] = '\0';
            }


            if (strcmp(name2, "from") == 0) {
                char* string = readFile(name3);

                if (strcmp(string, "1") == 0) {
                    printf("textFile acilamadi veya stringe deger atanamadi!");
                }
                else {
                    // dosyadan okunan string deger text mi int mi?
                    if (isDigit(string) == true) {

                        // "new" komutunun icerisine girmeyeceğinden createVariable atamasini burada yapiyoruz
                        char *variableName = name1;
                        // Yeni int değişkeni oluştur
                        createVariable(variables, variableName, 1);

                        // Statement'i dosya icindeki stringi verdigi degisken adina
                        // atama yapiyo olcak sekilde degistiriyoruz
                        // name1 := (okudugumuz dosyadaki int); -> seklinde atiyoruz statement'i
                        char stringWithAssignmentOperator[99999] = " := ";
                        char semiColon = ';';
                        strncat(stringWithAssignmentOperator, string, strlen(string));
                        strncat(name1, stringWithAssignmentOperator, strlen(stringWithAssignmentOperator));
                        strncat(name1, &semiColon, 1);

                        statement = name1;
                    }
                    else {

                        // "new" komutunun icerisine girmeyeceğinden createVariable atamasini burada yapiyoruz
                        char *variableName = name1;
                        // Yeni text değişkeni oluştur
                        createVariable(variables, variableName, 0);

                        // Dosyadan string okutuyoruz dolayısıyla aldigimiz degerin sonuna ve basina quote eklemek gerek
                        string = addQuotes(string);

                        // Statement'i dosya icindeki stringi verdigi degisken adina
                        // atama yapiyo olcak sekilde degistiriyoruz
                        // name1 := "(okudugumuz dosyadaki string)"; -> seklinde atiyoruz statement'i
                        char stringWithAssignmentOperator[99999] = " := ";
                        char semiColon = ';';
                        strncat(stringWithAssignmentOperator, string, strlen(string));
                        strncat(name1, stringWithAssignmentOperator, strlen(stringWithAssignmentOperator));
                        strncat(name1, &semiColon, 1);

                        statement = name1;
                    }
                }
            }
            else {
                printf("read komutu algılandı ama from komutu bulunamadi!");
            }
        }

            // write komutunu kontrol ediyor ve write kelimesinin string icersinde gecmedigini kontrol ediyor
        else if (strstr(statement, "write") != NULL & strstr(statement, "\"") == NULL) {

            removeChars(statement, 0, 4); // write kelimesini siler
            removeChars(statement, strlen(statement) - 1, strlen(statement) - 1); // ';' karakterini siler

            char text[strlen(statement)];
            strcpy(text, statement);
            char *tokencpy;
            char name1[99];
            char name2[99];
            char name3[99];
            char textVal[99999];

            tokencpy = strtok(statement, " ");
            if (tokencpy != NULL) {
                strncpy(name1, tokencpy, sizeof(name1));
                name1[sizeof(name1) - 1] = '\0';
            }

            tokencpy = strtok(NULL, " ");
            if (tokencpy != NULL) {
                strncpy(name2, tokencpy, sizeof(name2));
                name2[sizeof(name2) - 1] = '\0';
            }

            tokencpy = strtok(NULL, " ");
            if (tokencpy != NULL) {
                strncpy(name3, tokencpy, sizeof(name3));
                name3[sizeof(name3) - 1] = '\0';
            }

            if (strcmp(name2, "to") == 0) {

                // variables icersinden name1'in tuttugu string degiskenini bulup degerini atarız
                for (int i = 0; i < MAX_TOKEN_LENGTH; ++i) {
                    if (strcmp(variables[i].name, name1) == 0) {
                        strcpy(textVal, variables[i].value);
                        break;
                    }
                }

                // textVal string'inin sonuna \n ekliyoruz
                strncat(textVal, "\n", sizeof(textVal) - strlen(textVal) - 1);

                writeFile(textVal, name3);
            }
            else {
                printf("to komutu bulunamadi!\n");
            }
        }

            // input komutunu kontrol ediyor ve input kelimesinin string icersinde gecmedigini kontrol ediyor
        else if (strstr(statement, "input") != NULL & strstr(statement, "\"") == NULL) {

            removeChars(statement, 0, 4); // input kelimesini siler
            removeChars(statement, strlen(statement) - 1, strlen(statement) - 1); // ';' karakterini siler

            char text[strlen(statement)];
            strcpy(text, statement);
            char *tokencpy;
            char name1[99];
            char name2[99];
            char name3[200]; // promptText
            char keyboardInput[100];

            tokencpy = strtok(statement, " ");
            if (tokencpy != NULL) {
                strncpy(name1, tokencpy, sizeof(name1));
                name1[sizeof(name1) - 1] = '\0';
            }

            tokencpy = strtok(NULL, " ");
            if (tokencpy != NULL) {
                strncpy(name2, tokencpy, sizeof(name2));
                name2[sizeof(name2) - 1] = '\0';
            }

            tokencpy = strtok(NULL, " ");
            if (tokencpy != NULL) {
                strncpy(name3, tokencpy, sizeof(name3));
                name3[sizeof(name3) - 1] = '\0';
            }

            // promptText bir kac kelimeden olusuyorsa hepsini name3'e ekler
            tokencpy = strtok(NULL, " ");
            while (tokencpy) {
                strcat(name3, " ");
                strcat(name3, tokencpy);
                tokencpy = strtok(NULL, " ");
            }

            // command line uzerinden inputu alir
            getKeyboardInput(name3, keyboardInput);

            if (strcmp(name2, "prompt") == 0) {

                // gelen input int mi text mi?
                if (isDigit(keyboardInput) == true) {

                    // "new" komutunun icerisine girmeyeceğinden createVariable atamasini burada yapiyoruz
                    char *variableName = name1;
                    // Yeni int değişkeni oluştur
                    createVariable(variables, variableName, 1);

                    // Statement'i input icindeki int'i verdigi degisken adina
                    // atama yapiyo olacak sekilde degistiriyoruz
                    // name1 := (okudugumuz inputtaki int); -> seklinde atiyoruz statement'i
                    char stringWithAssignmentOperator[99999] = " := ";
                    char semiColon = ';';
                    strncat(stringWithAssignmentOperator, keyboardInput, strlen(keyboardInput));
                    strncat(name1, stringWithAssignmentOperator, strlen(stringWithAssignmentOperator));
                    strncat(name1, &semiColon, 1);

                    statement = name1;
                }
                else {

                    // "new" komutunun icerisine girmeyeceğinden createVariable atamasini burada yapiyoruz
                    char *variableName = name1;
                    // Yeni text değişkeni oluştur
                    createVariable(variables, variableName, 0);

                    // quote eklemeden once input sonunda newline komutu varsa siliyoruz
                    removeNewLine(keyboardInput);

                    // Dosyadan string okutuyoruz dolayısıyla aldigimiz degerin sonuna ve basina quote eklemek gerek
                    char* prompt = addQuotes(keyboardInput);

                    // Statement'i dosya icindeki stringi verdigi degisken adina
                    // atama yapiyo olcak sekilde degistiriyoruz
                    // name1 := "(okudugumuz dosyadaki string)"; -> seklinde atiyoruz statement'i
                    char stringWithAssignmentOperator[99999] = " := ";
                    char semiColon = ';';
                    strncat(stringWithAssignmentOperator, prompt, strlen(prompt));
                    strncat(name1, stringWithAssignmentOperator, strlen(stringWithAssignmentOperator));
                    strncat(name1, &semiColon, 1);

                    statement = name1;
                }
            }
            else {
                printf("prompt komutu bulunamadi!\n");
            }
        }

        char *token = strtok((char *) statement, " ");

        // Token'ın boş olmadığı kontrol ediliyor
        if (token != NULL) {

            // "new" komutunu kontrol ediyor
            if (strcmp(token, "new") == 0) {
                token = strtok(NULL, " ");  // değişken tipi

                // Değişken tipinin boş olmadığı kontrol ediliyor
                if (token != NULL) {
                    // "int" tipini kontrol ediyor
                    if (strcmp(token, "int") == 0) {
                        token = strtok(NULL, ";");  // variable name

                        // Değişken adının boş olmadığı kontrol ediliyor
                        if (token != NULL) {
                            char *variableName = token;
                            // Yeni int değişkeni oluştur
                            createVariable(variables, variableName, 1);
                        }
                            // Değişken adı eksik
                        else {
                            printf("Hata: Değişken adı eksik!\n");
                        }
                    }
                        // "text" tipini kontrol ediyor
                    else if (strcmp(token, "text") == 0) {
                        token = strtok(NULL, ";");  // variable name

                        // Değişken adının boş olmadığı kontrol ediliyor
                        if (token != NULL) {
                            char *variableName = token;
                            // Yeni text değişkeni oluştur
                            createVariable(variables, variableName, 0);
                        }
                            // Değişken adı eksik
                        else {
                            printf("Hata: Değişken adı eksik!\n");
                        }
                    }
                        // Geçersiz değişken tipi
                    else {
                        printf("Hata: Geçersiz değişken tipi!\n");
                    }
                }
                    // Değişken tipi eksik
                else {
                    printf("Hata: Değişken tipi eksik!\n");
                }
            }


                // "output" komutunu kontrol ediyor
            else if (strcmp(token, "output") == 0) {
                int variableType;
                token = strtok(NULL, ";");

                // Değişken adının boş olmadığı kontrol ediliyor
                if (token != NULL) {
                    char *variableName = token;

                    // İlgili değişkenin değerini ekrana yazdırma

                    // Değişkenin bulunup bulunmadığı kontrol ediliyor
                    if (variableName != NULL) {
                        for (int i = 0; i < MAX_TOKEN_LENGTH; ++i) {
                            if (strcmp(variables[i].name, variableName) == 0) {
                                variableType = variables[i].type;
                                break;
                            }
                        }

                        // Integer tipi
                        if (variableType == 1) {
                            char *value = getVariableValue(variables, variableName);
                            if (value != NULL) {
                                printf(value);
                                printf("\n");
                            } else {
                                printf("Hata: Değişken değeri bulunamadi!\n");
                            }
                        }
                            // Text tipi
                        else if (variableType == 0) {
                            char *value = getVariableValue(variables, variableName);
                            if (value != NULL) {
                                printf("%s\n", value);
                            } else {
                                printf("Hata: Değişken değeri bulunamadi!\n");
                            }
                        }
                    }
                        // Geçersiz değişken adı
                    else {
                        printf("Hata: Geçersiz değişken adı!\n");
                    }
                }
                    // Değişken adı eksik
                else {
                    printf("Hata: Değişken adı eksik!\n");
                }
            }



                // size() keyword'unu kontrol ediyor
            else if (strstr(token, "size") != NULL) {
                char innerString[MAX_IDENTIFIER_SIZE]; // parantez icindeki degeri tutar
                getInnerStringInParentheses(token, innerString); // parantez icindeki degeri parse eder

                char innerStringValue[999]; // innerString icindeki degiskenin tuttugu degeri tutar

                // innerString'in tuttugu degiskenin degerini innerStringValue'e atar
                for (int k = 0; k < MAX_TOKEN_LENGTH; ++k) {
                    if (strcmp(variables[k].name, innerString) == 0) {
                        strcpy(innerStringValue, variables[k].value);
                        break;
                    }
                }

                // innerStringValue icerisinde tuttugumuz stringin baş ve son quote'larini siler
                removeQuotes(innerStringValue);

                int size = strlen(innerStringValue); // string uzunlugunu bulur

                printf("Size: %d\n", size);
            }

                //subs() keywordunu kontrol ediyor
            else if (strstr(token, "subs") != NULL) {

                removeChars(token, 0, 4); // İlk 5 karakteri sil
                removeChars(token, strlen(token) - 2, strlen(token) - 1); // Son 2 karakteri sil


                char text[strlen(token)];
                strcpy(text, token);
                char *tokencpy;
                char name1[99];
                char name2[99];
                char name3[99];

                tokencpy = strtok(token, ",");
                if (tokencpy != NULL) {
                    strncpy(name1, tokencpy, sizeof(name1));
                    name1[sizeof(name1) - 1] = '\0';
                }

                tokencpy = strtok(NULL, ",");
                if (tokencpy != NULL) {
                    strncpy(name2, tokencpy, sizeof(name2));
                    name2[sizeof(name2) - 1] = '\0';
                }

                tokencpy = strtok(NULL, ",");
                if (tokencpy != NULL) {
                    strncpy(name3, tokencpy, sizeof(name3));
                    name3[sizeof(name3) - 1] = '\0';
                }
                int begin = atoi(&name2);
                int end = atoi(&name3);


                atamaIslemi(variables, name1);
                removeQuotes(name1);

                if (strlen(name1) > end && strlen(name1) > begin && end > begin && begin>=0 && end>=1) {
                    subs(name1, begin, end);

                    printf("Subs: %s\n", name1);
                } else {

                    printf("HATA! Hatali subs komutu kullanimi!");
                }



            }

                // locate() keyword'unu kontrol ediyor
            else if (strstr(token, "locate") != NULL) {

                removeChars(token, 0, 6); // İlk 7 karakteri sil
                removeChars(token, strlen(token) - 2, strlen(token) - 1); // Son 2 karakteri sil


                char text[strlen(token)];
                strcpy(text, token);
                char *tokencpy;
                char name1[99];
                char name2[99];
                char name3[99];

                tokencpy = strtok(token, ",");
                if (tokencpy != NULL) {
                    strncpy(name1, tokencpy, sizeof(name1));
                    name1[sizeof(name1) - 1] = '\0';
                }

                tokencpy = strtok(NULL, ",");
                if (tokencpy != NULL) {
                    strncpy(name2, tokencpy, sizeof(name2));
                    name2[sizeof(name2) - 1] = '\0';
                }

                tokencpy = strtok(NULL, ",");
                if (tokencpy != NULL) {
                    strncpy(name3, tokencpy, sizeof(name3));
                    name3[sizeof(name3) - 1] = '\0';
                }

                int start = atoi(&name3);
                atamaIslemi(variables, name1);
                atamaIslemi(variables, name2);
                removeQuotes(name1);
                removeQuotes(name2);

                if(start >= 0 && start <= strlen(name1)-1) {

                    int position = locate(name1, name2, start);

                    if (position != 0)
                        printf("Locate: %d\n", position);
                    else
                        printf("Pozisyon bulunamadi.\n");
                }
                else{
                    printf("HATA! Yanlış locate komutu kullanımı!");
                }


            }

                // insert() keyword'unu kontrol ediyor
            else if (strstr(token, "insert") != NULL) {

                removeChars(token, 0, 6); // İlk 7 karakteri sil
                removeChars(token, strlen(token) - 2, strlen(token) - 1); // Son 2 karakteri sil


                char text[strlen(token)];
                strcpy(text, token);
                char *tokencpy;
                char name1[99];
                char name2[99];
                char name3[99];

                tokencpy = strtok(token, ",");
                if (tokencpy != NULL) {
                    strncpy(name1, tokencpy, sizeof(name1));
                    name1[sizeof(name1) - 1] = '\0';
                }

                tokencpy = strtok(NULL, ",");
                if (tokencpy != NULL) {
                    strncpy(name2, tokencpy, sizeof(name2));
                    name2[sizeof(name2) - 1] = '\0';
                }

                tokencpy = strtok(NULL, ",");
                if (tokencpy != NULL) {
                    strncpy(name3, tokencpy, sizeof(name3));
                    name3[sizeof(name3) - 1] = '\0';
                }

                int index = atoi(&name2);
                atamaIslemi(variables, name1);
                atamaIslemi(variables, name3);
                removeQuotes(name1);
                removeQuotes(name3);

                if(index >= 0) {

                    char* result = insert(name1, index, name3);

                    if (result != NULL) {
                        printf("Insert: %s\n", result);
                        free(result);
                    } else {
                        printf("Insert işlemi başarısız.\n");
                    }

                }
                else{
                    printf("HATA! Yanlış insert komutu kullanımı!");
                }


            }

                // override() keyword'unu kontrol ediyor
            else if (strstr(token, "override") != NULL) {

                removeChars(token, 0, 8); // İlk 9 karakteri sil
                removeChars(token, strlen(token) - 2, strlen(token) - 1); // Son 2 karakteri sil


                char text[strlen(token)];
                strcpy(text, token);
                char *tokencpy;
                char name1[99];
                char name2[99];
                char name3[99];

                tokencpy = strtok(token, ",");
                if (tokencpy != NULL) {
                    strncpy(name1, tokencpy, sizeof(name1));
                    name1[sizeof(name1) - 1] = '\0';
                }

                tokencpy = strtok(NULL, ",");
                if (tokencpy != NULL) {
                    strncpy(name2, tokencpy, sizeof(name2));
                    name2[sizeof(name2) - 1] = '\0';
                }

                tokencpy = strtok(NULL, ",");
                if (tokencpy != NULL) {
                    strncpy(name3, tokencpy, sizeof(name3));
                    name3[sizeof(name3) - 1] = '\0';
                }

                int index = atoi(&name2);
                atamaIslemi(variables, name1);
                atamaIslemi(variables, name3);
                removeQuotes(name1);
                removeQuotes(name3);

                if(index >= 0) {

                    override(name1, index, name3);

                    printf("Override: %s\n", name1);


                }
                else{
                    printf("HATA! Yanlış override komutu kullanımı!");
                }


            }

                // asString() komutu
            else if (strstr(token, "asString") != NULL) {

                removeChars(token, 0, 8); // İlk 9 karakteri sil
                removeChars(token, strlen(token) - 2, strlen(token) - 1); // Son 2 karakteri sil

                char myIntStr[99];
                int myInt;
                atamaIslemi(variables, token);
                if(isDigit(token) == true) {
                    int myInt = atoi(&myIntStr);
                    char *str = asString(myInt);
                    printf("asString: \"%s\"\n", str);
                }
                else{
                    printf("Hata! Girdi harf içeriyor!");
                }



            }
                //asText komutu
            else if (strstr(token, "asText") != NULL) {

                removeChars(token, 0, 6); // İlk 7 karakteri sil
                removeChars(token, strlen(token) - 2, strlen(token) - 1); // Son 2 karakteri sil


                atamaIslemi(variables, token);

                if(isDigit(token) != false) {
                    int myInt = asText(token);

                    printf("asText: %d\n", myInt);

                }
                else{
                    printf("Hata! Girdi sayı içermiyor!");
                }



            }


                // Atama komutunu kontrol ediyor
            else {
                int variableType;
                char *variableName = token;

                // variableType'a degerini atiyoruz
                for (int i = 0; i < MAX_TOKEN_LENGTH; ++i) {

                    if (strcmp(variables[i].name, variableName) == 0) {
                        variableType = variables[i].type;
                        break;
                    }
                }

                token = strtok(NULL, " ");

                // ":=" operatörünün boş olmadığı kontrol ediliyor
                if (token != NULL && strcmp(token, ":=") == 0) {
                    token = strtok(NULL, ";");
                    // İfade içerisinde '+' işareti varsa
                    if (strstr((char *) token, "+") != NULL && strstr((char *) token, "\"") != NULL) {//Birleştirme işlemi  // str := "hello" + "world";


                        char tokencpy[strlen(token)];
                        strcpy(tokencpy, token);
                        int len = strlen(token);
                        int i, j;

                        for (i = 0, j = 0; i < len; i++) {
                            if (tokencpy[i] != '\"') {
                                tokencpy[j] = tokencpy[i];
                                j++;
                            }

                        }
                        for (int k = j; k < strlen(token); ++k) {
                            tokencpy[k] = '\0';
                        }


                        char *str1 = strtok(tokencpy, "+");
                        char *str2 = strtok(NULL, "+");

                        char result[999];

                        if (str1 != NULL && str2 != NULL) {
                            strcpy(result, str1);
                            strcat(result, str2);


                            for (int k = 0; k < MAX_TOKEN_LENGTH; ++k) {
                                if (strcmp(variables[k].name, variableName) == 0) {
                                    strcpy(variables[k].value, result);
                                    break;
                                }
                            }

                        } else {
                            printf("Geçersiz ifade!\n");
                        }
                    }


                    else if (strstr((char *) token, "-") != NULL && strstr((char *) token, "\"") != NULL) {// str := "world" - "wo";

                        char tokencpy[strlen(token)];
                        strcpy(tokencpy, token);
                        int len = strlen(token);
                        int i, j;

                        for (i = 0, j = 0; i < len; i++) {
                            if (tokencpy[i] != '\"') {
                                tokencpy[j] = tokencpy[i];
                                j++;
                            }

                        }
                        for (int k = j; k < strlen(token); ++k) {
                            tokencpy[k] = '\0';
                        }

                        for (i = 0, j = 0; i < len; i++) {
                            if (tokencpy[i] != ' ') {
                                tokencpy[j] = tokencpy[i];
                                j++;
                            }

                        }
                        for (int k = j; k < strlen(token); ++k) {
                            tokencpy[k] = '\0';
                        }


                        char *str1 = strtok(tokencpy, "-");
                        char *str2 = strtok(NULL, "-");

                        char sonuc[99];

                        string_cikar(str1, str2,sonuc);



                        for (int k = 0; k < MAX_TOKEN_LENGTH; ++k) {
                            if (strcmp(variables[k].name, variableName) == 0) {
                                strcpy(variables[k].value, sonuc);
                                break;
                            }
                        }

                    }


                    else if (strstr((char *) token, "+") != NULL && strstr((char *) token, "\"") == NULL && isDigit(token) == true) {  //int := 5 + 3;

                        char tokencpy[strlen(token)];
                        strcpy(tokencpy, token);
                        int len = strlen(token);
                        int i, j;

                        for (i = 0, j = 0; i < len; i++) {
                            if (tokencpy[i] != ' ') {
                                tokencpy[j] = tokencpy[i];
                                j++;
                            }

                        }
                        for (int k = j; k < strlen(token); ++k) {
                            tokencpy[k] = '\0';
                        }

                        char *int1 = strtok(tokencpy, "+");
                        char *int2 = strtok(NULL, "+");


                        int num1 = atoi(int1); // char* -> int dönüşümü için atoi kullanılır
                        int num2 = atoi(int2);

                        int addition = num1 + num2;
                        char additionStr[999]; // String için yeterli boyutta bir dizi tanımlanır
                        sprintf(additionStr, "%d", addition); // int'i stringe çevirerek additionStr'ye kaydeder


                        for (int k = 0; k < MAX_TOKEN_LENGTH; ++k) {
                            if (strcmp(variables[k].name, variableName) == 0) {
                                strcpy(variables[k].value, additionStr);
                                break;
                            }
                        }

                    }

                    else if (strstr((char *) token, "-") != NULL && strstr((char *) token, "\"") == NULL && isDigit(token) == true) {  //int := 5 - 3;

                        char tokencpy[strlen(token)];
                        strcpy(tokencpy, token);
                        int len = strlen(token);
                        int i, j;

                        for (i = 0, j = 0; i < len; i++) {
                            if (tokencpy[i] != ' ') {
                                tokencpy[j] = tokencpy[i];
                                j++;
                            }

                        }
                        for (int k = j; k < strlen(token); ++k) {
                            tokencpy[k] = '\0';
                        }

                        char *int1 = strtok(tokencpy, "-");
                        char *int2 = strtok(NULL, "-");


                        int num1 = atoi(int1); // char* -> int dönüşümü için atoi kullanılır
                        int num2 = atoi(int2);
                        if(num1 >= num2) {
                            int addition = num1 - num2;
                            char additionStr[999]; // String için yeterli boyutta bir dizi tanımlanır
                            sprintf(additionStr, "%d", addition); // int'i stringe çevirerek additionStr'ye kaydeder


                            for (int k = 0; k < MAX_TOKEN_LENGTH; ++k) {
                                if (strcmp(variables[k].name, variableName) == 0) {
                                    strcpy(variables[k].value, additionStr);
                                    break;
                                }
                            }
                        }
                        else{
                            printf("HATA: Negatif sonuç!");
                        }

                    }


                    else if (strstr((char *) token, "+") != NULL && strstr((char *) token, "\"") == NULL && variableType == 0) { //str1+str2


                        char tokencpy[strlen(token)];
                        strcpy(tokencpy, token);
                        int len = strlen(token);
                        int i, j;

                        for (i = 0, j = 0; i < len; i++) {
                            if (tokencpy[i] != ' ') {
                                tokencpy[j] = tokencpy[i];
                                j++;
                            }

                        }
                        for (int k = j; k < strlen(token); ++k) {
                            tokencpy[k] = '\0';
                        }

                        char *value1 = strtok(tokencpy, "+");
                        char *value2 = strtok(NULL, "+");


                        bool flag1 = false;

                        for (int k = 0; k < MAX_TOKEN_LENGTH; ++k) {


                            if(((strcmp(variables[k].name,value1) == 0) || (strcmp(variables[k].name,value2) == 0)) || flag1 == true){

                                if(strcmp(variables[k].name,value1) == 0){
                                    value1 = variables[k].value;
                                    flag1 = true;



                                }
                                else if(strcmp(value2,variables[k].name)==0) {
                                    value2 = variables[k].value;
                                    flag1 = true;
                                }

                                if((strcmp(variables[k].name,value1) != 0) && (strcmp(variables[k].name,value2) == 0)) {
                                    if (strcmp(value1, variables[k].name) == 0) {
                                        value1 = variables[k].value;

                                    } else if (strcmp(value2, variables[k].name) == 0) {
                                        value2 = variables[k].value;

                                    }
                                }

                                char resultt[999];

                                strcpy(resultt, value1);
                                strcat(resultt, value2);

                                int len = strlen(resultt);
                                int i, j;

                                for (i = 0, j = 0; i < len; i++) {
                                    if (resultt[i] != '\"') {
                                        resultt[j] = resultt[i];
                                        j++;
                                    }
                                }

                                resultt[j] = '\0';


                                for (int k = 0; k < MAX_TOKEN_LENGTH; ++k) {
                                    if (strcmp(variables[k].name, variableName) == 0) {
                                        strcpy(variables[k].value, resultt);
                                        break;
                                    }
                                }
                            }
                        }
                    }



                    else if (strstr((char *) token, "-") != NULL && strstr((char *) token, "\"") == NULL && variableType == 0) { //str1-str2


                        char tokencpy[strlen(token)];
                        strcpy(tokencpy, token);
                        int len = strlen(token);
                        int i, j;
                        for (i = 0, j = 0; i < len; i++) {
                            if (tokencpy[i] != ' ') {
                                tokencpy[j] = tokencpy[i];
                                j++;
                            }

                        }
                        for (int k = j; k < strlen(token); ++k) {
                            tokencpy[k] = '\0';
                        }


                        char *value1 = strtok(tokencpy, "-");
                        char *value2 = strtok(NULL, "-");


                        bool flag1 = false;

                        for (int k = 0; k < MAX_TOKEN_LENGTH; ++k) {


                            if(((strcmp(variables[k].name,value1) == 0) || (strcmp(variables[k].name,value2) == 0)) || flag1 == true){

                                if(strcmp(variables[k].name,value1) == 0){
                                    value1 = variables[k].value;
                                    flag1 = true;
                                    int len = strlen(value1);
                                    int i, j;

                                    for (i = 0, j = 0; i < len; i++) {
                                        if (value1[i] != '\"') {
                                            value1[j] = value1[i];
                                            j++;
                                        }

                                    }
                                    for (int k = j; k < strlen(value1); ++k) {
                                        value1[k] = '\0';
                                    }



                                }
                                else if(strcmp(value2,variables[k].name)==0) {
                                    value2 = variables[k].value;
                                    flag1 = true;
                                    int len = strlen(value2);
                                    int i, j;

                                    for (i = 0, j = 0; i < len; i++) {
                                        if (value2[i] != '\"') {
                                            value2[j] = value2[i];
                                            j++;
                                        }

                                    }
                                    for (int k = j; k < strlen(value2); ++k) {
                                        value2[k] = '\0';
                                    }
                                }

                                if((strcmp(variables[k].name,value1) != 0) && (strcmp(variables[k].name,value2) == 0)) {
                                    if (strcmp(value1, variables[k].name) == 0) {
                                        value1 = variables[k].value;
                                        int len = strlen(value1);
                                        int i, j;

                                        for (i = 0, j = 0; i < len; i++) {
                                            if (value1[i] != '\"') {
                                                value1[j] = value1[i];
                                                j++;
                                            }

                                        }
                                        for (int k = j; k < strlen(value1); ++k) {
                                            value1[k] = '\0';
                                        }

                                    } else if (strcmp(value2, variables[k].name) == 0) {
                                        value2 = variables[k].value;
                                        int len = strlen(value2);
                                        int i, j;

                                        for (i = 0, j = 0; i < len; i++) {
                                            if (value2[i] != '\"') {
                                                value2[j] = value2[i];
                                                j++;
                                            }

                                        }
                                        for (int k = j; k < strlen(value2); ++k) {
                                            value2[k] = '\0';
                                        }

                                    }

                                }
                                char sonuc[99];

                                string_cikar(value1, value2,sonuc);



                                for (int k = 0; k < MAX_TOKEN_LENGTH; ++k) {
                                    if (strcmp(variables[k].name, variableName) == 0) {
                                        strcpy(variables[k].value, sonuc);
                                        break;
                                    }
                                }



                            }
                        }
                    }


                        // int toplama yapıyor i3 = i1 + i2; şeklinde
                    else if (strstr((char *) token, "+") != NULL && strstr((char *) token, "\"") == NULL && variableType == 1) {

                        char tokencpy[strlen(token)];
                        strcpy(tokencpy, token);
                        int len = strlen(token);
                        int i, j;

                        for (i = 0, j = 0; i < len; i++) {
                            if (tokencpy[i] != ' ') {
                                tokencpy[j] = tokencpy[i];
                                j++;
                            }

                        }
                        for (int k = j; k < strlen(token); ++k) {
                            tokencpy[k] = '\0';
                        }

                        char *value1 = strtok(tokencpy, "+");
                        char *value2 = strtok(NULL, "+");

                        bool flag1 = false;

                        for (int k = 0; k < MAX_TOKEN_LENGTH; ++k) {


                            if(((strcmp(variables[k].name,value1) == 0) || (strcmp(variables[k].name,value2) == 0)) || flag1 == true){

                                if(strcmp(variables[k].name,value1) == 0){
                                    value1 = variables[k].value;
                                    flag1 = true;

                                }
                                else if(strcmp(value2,variables[k].name)==0) {
                                    value2 = variables[k].value;
                                    flag1 = true;
                                }

                                if((strcmp(variables[k].name,value1) != 0) && (strcmp(variables[k].name,value2) == 0)) {
                                    if (strcmp(value1, variables[k].name) == 0) {
                                        value1 = variables[k].value;

                                    } else if (strcmp(value2, variables[k].name) == 0) {
                                        value2 = variables[k].value;

                                    }
                                }



                                int intValue1 = atoi(value1);
                                int intValue2 = atoi(value2);

                                int intResult = intValue1 + intValue2;

                                char Resulttt[999];

                                sprintf(Resulttt, "%d", intResult);

                                for (int k = 0; k < MAX_TOKEN_LENGTH; ++k) {
                                    if (strcmp(variables[k].name, variableName) == 0) {
                                        strcpy(variables[k].value, Resulttt);
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    else if (strstr((char *) token, "-") != NULL && strstr((char *) token, "\"") == NULL && variableType == 1) {  // int toplama yapıyor i3 = i1 - i2; şeklinde

                        char tokencpy[strlen(token)];
                        strcpy(tokencpy, token);
                        int len = strlen(token);
                        int i, j;

                        for (i = 0, j = 0; i < len; i++) {
                            if (tokencpy[i] != ' ') {
                                tokencpy[j] = tokencpy[i];
                                j++;
                            }

                        }
                        for (int k = j; k < strlen(token); ++k) {
                            tokencpy[k] = '\0';
                        }

                        char *value1 = strtok(tokencpy, "-");
                        char *value2 = strtok(NULL, "-");

                        bool flag1 = false;

                        for (int k = 0; k < MAX_TOKEN_LENGTH; ++k) {


                            if(((strcmp(variables[k].name,value1) == 0) || (strcmp(variables[k].name,value2) == 0)) || flag1 == true) {

                                if (strcmp(variables[k].name, value1) == 0) {
                                    value1 = variables[k].value;
                                    flag1 = true;

                                } else if (strcmp(value2, variables[k].name) == 0) {
                                    value2 = variables[k].value;
                                    flag1 = true;
                                }

                                if ((strcmp(variables[k].name, value1) != 0) &&
                                    (strcmp(variables[k].name, value2) == 0)) {
                                    if (strcmp(value1, variables[k].name) == 0) {
                                        value1 = variables[k].value;

                                    } else if (strcmp(value2, variables[k].name) == 0) {
                                        value2 = variables[k].value;

                                    }
                                }


                                int intValue1 = atoi(value1);
                                int intValue2 = atoi(value2);

                                if (intValue1 >= intValue2) {
                                    int intResult = intValue1 - intValue2;

                                    char Resulttt[999];

                                    sprintf(Resulttt, "%d", intResult);

                                    for (int k = 0; k < MAX_TOKEN_LENGTH; ++k) {
                                        if (strcmp(variables[k].name, variableName) == 0) {
                                            strcpy(variables[k].value, Resulttt);
                                            break;
                                        }
                                    }
                                }
                                else{
                                    printf("HATA!: Negatif sonuç!");
                                }
                            }
                        }
                    }

                        // Değerin boş olmadığı kontrol ediliyor
                    else if (token != NULL) {


                        for (int i = 0; i < MAX_TOKEN_LENGTH; ++i) {

                            if (strcmp(variables[i].name, variableName) == 0) {
                                variableType = variables[i].type;
                                break;

                            }

                        }

                        // Integer tipini kontrol ediyor
                        if (variableType == 1) {
                            char valueStr[999];
                            int value = atoi(token);
                            sprintf(valueStr, "%d", value);
                            // Değişkene int değer atama
                            assignVariable(variables, variableName, variableType, &valueStr);
                        }
                            // Text tipini kontrol ediyor
                        else if (variableType == 0 && strstr(token,"\"") != NULL) {

                            char *value = token;
                            // Değişkene metin değeri atama
                            assignVariable(variables, variableName, variableType, value);


                        }
                        else if (variableType == 0 && strstr(token,"\"") == NULL) {
                            printf("Hata: Tirnak isareti unutulmus!\n");
                        }
                        else {
                            printf("Hata: Geçersiz değişken tipi!\n");
                        }


                    }
                        // Atama işlemi eksik
                    else {
                        printf("Hata: Atama işlemi eksik!\n");
                    }
                }
            }
            // Geçersiz komut


        }
    }
}

void deleteTrailingSpaces(char* str) {
    int length = strlen(str);
    int i = length - 1;

    while (i >= 0 && isspace(str[i])) {
        str[i] = '\0';
        i--;
    }
}

int clearComment(char* statement, int isInComment) {
    char* start = strstr(statement, "/*"); // yorum satirinin baslangicini tutar
    char* end = strstr(statement, "*/");   // yorum satirinin sonunu tutar

    if (start != NULL) {
        if (end != NULL & end > start) {
            // yorum satiri bulunup statement icinde bittiginde
            // yorumu karakter shift ederek siler
            memmove(start, end + 2, strlen(end + 2) + 1);

            // sonda bosluk kaldiysa siler
            deleteTrailingSpaces(statement);

            return 0;
        }
        else {
            // yorum satiri bulundugunda ama statement icerisinde bitmediginde
            // yorum baslangicinden sonrasini siler
            *start = '\0';

            // sonda bosluk kaldiysa siler
            deleteTrailingSpaces(statement);

            // 1 dondurur yorum satirinin bir sonraki asamada devam ettigini anlamak icin
            return 1;
        }
    }
    else if (isInComment == 1) { // bitmemis yorum satirinin devam statementina gecildigi durumda

        // yorum satirinin son satiri olup olmadigini kontrol eder
        if (end != NULL) {
            // yorum satirinin sonu ve öncesini siler
            memmove(statement, end + 2, strlen(end + 2) + 1);

            // sonda bosluk kaldiysa siler
            deleteTrailingSpaces(statement);

            return 0;
        }
        else { // son satiri degilse tum satiri siler ve 1 return eder
            statement[0] = '\0';
            return 1;
        }

    }

    // yorum satiri bulunmadi
    return 0;
}


int main(int argc, char* argv[]) {

    // cli uzerinden arguman dosyasi aliyoruz
    char* fileName = argv[1];

    char statement[999];
    Variable variables[MAX_TOKEN_LENGTH];
    memset(variables, 0, sizeof(variables));

    FILE* inputFile = fopen(fileName, "r");
    if (inputFile == NULL) {
        printf("Hata: Girdi dosyası açılamadı!\n");
        return 1;
    }

    int isInComment = 0;

    while (fgets(statement, sizeof(statement), inputFile) != NULL) {
        statement[strcspn(statement, "\n")] = '\0';

        // clearComment yorumlari statement icerisinden temizler
        // eger yorum tek satirda bitmiyorsa 1 degerini cevirir bitiyorsa 0
        isInComment = clearComment(statement, isInComment);

        interpretStatement(variables, statement);
    }

    fclose(inputFile);


    while (1) {
        fgets(statement, sizeof(statement), stdin);

        // Yeni satır karakterini kaldır
        statement[strcspn(statement, "\n")] = '\0';

        if (strcmp(statement, "exit") == 0) {
            break;
        }

        interpretStatement(variables, statement);
    }

    // Belleği temizle
    for (int i = 0; i < MAX_TOKEN_LENGTH; i++) {
        if (variables[i].type == 0) {
            free(variables[i].value);
        } else if (variables[i].type == 1) {
            free(variables[i].value);
        }
    }

    return 0;
}