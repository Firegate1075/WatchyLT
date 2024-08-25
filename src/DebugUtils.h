#define DEBUG

#ifdef DEBUG
#define debugBegin() Serial.begin(115200)
#define debugPrint(x) \
    Serial.print(x);  \
    Serial.flush();
#define debugPrintln(x) \
    Serial.println(x);  \
    Serial.flush();
#define debugPrintf(...)        \
    Serial.printf(__VA_ARGS__); \
    Serial.flush();
#else
#define debugBegin()
#define debugPrint(x)
#define debugPrintln(x)
#define debugPrintf(...)
#endif