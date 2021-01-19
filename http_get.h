int stringkey(char *buf, char *name, char *val, int size);
int numkey(char *buf, char *name, double val, int size);
int header(char *buf, char *host, int port, int size);

int strlcpy(char *dst, const char *src, int size);
int itostr(char *dst, long x, int size);
int ftostr(char *dst, double x, int size);
