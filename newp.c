#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>

char *dicbuf_h;

int setup_dictionary(char *f_name, char ***dicarr)
{
	struct stat st;
	char **dicarr_r;
	int word_count_h = 0;
	int flag = 0, m = 0;

	FILE *fp = fopen(f_name, "r");
	if (!fp)
		perror("Failed to open file");
	stat(f_name, &st);

	dicbuf_h = (char *)malloc(st.st_size + 1);

	fread(dicbuf_h, sizeof(char), st.st_size, fp);
	dicbuf_h[st.st_size] = '\0';

	for (int i = 0; i < st.st_size; i++) {
		if (dicbuf_h[i] == '\n' || dicbuf_h[i] == '\r') {
			dicbuf_h[i] = 0;
			flag = 0;
		} else {
			if (flag == 0)
				word_count_h++;
			flag = 1;
		}

	}

	*dicarr = malloc(word_count_h * 8);
	dicarr_r = *dicarr;

	flag = 0;
	for (int i = 0; i < st.st_size; i++) {
		if (dicbuf_h[i] == 0) {
			flag = 0;
		} else {
			if (flag == 0) {
				dicarr_r[m] = dicbuf_h + i;
				m++;
			}
			flag = 1;
		}
	}

	//printf("word_count_h:%d\n", word_count_h);
	//printf("malloc:%d\n", word_count_h * 8);

	return word_count_h;

}

bool binary_search(char *word, int word_count_h, char **dicarr)
{
	int l = 0;
	int r = word_count_h - 1;
	int mi;
	//printf("%s:\n", arr_r[i]);

	while (1) {
		if (l <= r) {
			mi = (l + r + 1) / 2;

			if (strcmp(word, dicarr[mi]) == 0) {
				return true;
				break;
			} else if (strcmp(word, dicarr[mi]) < 0) {
				r = mi - 1;
			} else if (strcmp(word, dicarr[mi]) > 0) {
				l = mi + 1;
			}

		} else
			break;
	}
	return false;
}

double matching(int count, int match_count)
{
	double match_p;
	double a = count * 1;
	double b = match_count * 1;

	match_p = (b / a) * 100;

	return match_p;

}

int slice(char *str, char ***arr)
{
	int count = 0;
	int flag = 0, m = 0;
	char **arr_r;
	int len = strlen(str);

	for (int i = 0; i < len; i++) {
		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\r') {
			str[i] = 0;
			flag = 0;
		} else {
			if (flag == 0)
				count++;
			flag = 1;
		}
	}
	printf("%d\n", count);

	*arr = malloc(count * 8);
	arr_r = *arr;

	flag = 0;
	for (int i = 0; i < len; i++) {
		if (str[i] == 0) {
			flag = 0;
		} else {
			if (flag == 0) {
				arr_r[m] = str + i;
				m++;
			}
			flag = 1;
		}
	}
	if (count >= 7) {
		//if (strcmp(arr_r[6], ":") == 0) { arr_r가ㅣ 이차원 퍼인ㅌ라서 그럼
		if (*arr_r[6] == ':') {
			for (int i = 7; i < count; i++) {
				printf("\"%s\" \n", arr_r[i]);

			}

		}
	}
//      for (int i = 0; i < count; i++)
	//                 printf("%s",arr_r[i]);

	return count;
}

int main(int argc, char *argv[])
{
	int word_count_h;
	char str[100];
	int i = 0;
	char **words;
	char *testf_name;
	testf_name = argv[2];
	int match_count = 0;
	double match_p = 0;
	int count = 0;
	char **dicarr;
	int allcount=0;

	word_count_h = setup_dictionary(argv[1], &dicarr);

	FILE *fp2 = fopen(testf_name, "r");
	if (fp2 != NULL) {
		while (feof(fp2) == 0) {
			if (fgets(str, 100, fp2) != NULL) {
				count = slice(str, &words);
				if (count >= 7 && *words[6] == ':') {
					for (i = 7; i < count; i++) {
						allcount++;
						if (binary_search(words[i], word_count_h, dicarr)){
							printf("y\n");
							match_count++;
						}else
							printf("n\n");
					}
				}
				free(words);
			}
		}

	}

	printf("allcount = %d, match = %d\n", allcount, match_count);
	match_p = matching(allcount, match_count);
	printf("match_point:%f\n", match_p);

	fclose(fp2);
	free(dicarr);
	free(dicbuf_h);
	return 0;

}
