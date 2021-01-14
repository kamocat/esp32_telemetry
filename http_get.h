char *stringkey(char *buf, int *size, char *name, char *val);
char *numkey(char *buf, int *len, char *name, double val);
char *header(char *buf, int *len, char *host, int port);

int strlcpy(char *dst, const char *src, int size);
int itostr(char *dst, long x, int size);
int ftostr(char *dst, double x, int size);