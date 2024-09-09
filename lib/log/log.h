//
// Created by linhf on 8/23/24.
//

#ifndef FIP_LOG_H
#define FIP_LOG_H


class log {
    log() {
        writer = defaultWriter;
    }
    log(void (*writer)(char * logStr)) {
        this->writer = writer;
    }
private:
    static void defaultWriter(char *logStr);
    static void (*writer)(char * logStr);
public:
    static void Println(char* logStr) {
        writer(logStr);
    }

    static void Warnln(char* logStr) {
        writer(logStr);
    }

    static void Errorln(char* logStr) {
        writer(logStr);
    }

    static void Panicln(char* logStr) {
        writer(logStr);
    }
};


#endif //FIP_LOG_H
