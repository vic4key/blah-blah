int v1 = 1;
int v2 = 2;

const int* pb = &v1;
pb[0] = 1; // error change value

int* const pa = &v2;
pa = &v2; // error change pointer
