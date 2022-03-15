
// 0 = Not the same
int compare_strings(const char str1[], const char str2[]) {
	int cur_pos = 0;

	while (str1[cur_pos] != '\0' && str2[cur_pos] != '\0') {
		if (str1[cur_pos] != str2[cur_pos]) {
			return 0;
		}

		cur_pos++;	// Increase current position in the arrays
	}

	return 1;
}
