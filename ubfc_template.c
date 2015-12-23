#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BF_CHUNK_SIZE 30000
#define BF_ERRCODE_NOMEM 1
#define BF_ERRCODE_EOF 2
#define BF_ERRCODE_BOUND_VIOLATION 3
#ifdef BF_DEBUG
#define BF_ERRCODE_RUNTIME_ASSERT 5
#endif
#define BF_FILENAME_BUFFERSIZE 256
#define PROGRAM_LEN sizeof(program) / sizeof(unsigned char)

struct bf_chunk {
	unsigned char data[BF_CHUNK_SIZE];
	struct bf_chunk *left;
	struct bf_chunk *right;
};

int chunk_pos, print_mode, print_pos;
struct bf_chunk *current_chunk;

unsigned long long cursor;
unsigned char filename_buffer[BF_FILENAME_BUFFERSIZE];

inline struct bf_chunk* bf_alloc_chunk(struct bf_chunk* left, struct bf_chunk* right) {
	struct bf_chunk* ptr = (struct bf_chunk*)malloc(sizeof(struct bf_chunk));
	if (!ptr) exit(BF_ERRCODE_NOMEM);
	memset(ptr, 0, sizeof(struct bf_chunk));
	ptr->left = left;
	ptr->right = right;
	return ptr;
}

inline void bf_init() {
	current_chunk = bf_alloc_chunk(0, 0);
	chunk_pos = BF_CHUNK_SIZE / 2;
	print_mode = 0;
}

inline void bf_left() {
	if (chunk_pos)
		--chunk_pos;
	else {
		if (!current_chunk->left)
			current_chunk->left = bf_alloc_chunk(0, current_chunk);
		current_chunk = current_chunk->left;
		chunk_pos = BF_CHUNK_SIZE - 1;
	}
}

inline void bf_right() {
	if (++chunk_pos >= BF_CHUNK_SIZE) {
		chunk_pos = 0;
		if (!current_chunk->right)
			current_chunk->right = bf_alloc_chunk(current_chunk, 0);
		current_chunk = current_chunk->right;
	}
}

inline void bf_inc() {
	++current_chunk->data[chunk_pos];
}

inline void bf_dec() {
	--current_chunk->data[chunk_pos];
}

inline void bf_print() {
	if (print_mode) {
		filename_buffer[print_pos++] = current_chunk->data[chunk_pos];
		if (print_pos >= BF_FILENAME_BUFFERSIZE) print_pos = 0;
	} else
		putchar(current_chunk->data[chunk_pos]);
}

inline void bf_read() {
	int charcode = getchar();
	if (charcode == EOF) exit(BF_ERRCODE_EOF);
	current_chunk->data[chunk_pos] = (unsigned char)charcode;
}

inline void bf_validate_cursor() {
	if (cursor >= PROGRAM_LEN)
		exit(BF_ERRCODE_BOUND_VIOLATION);
}

inline void bf_cycle_start() {
	cursor += sizeof(unsigned long long);
	bf_validate_cursor();
	if (current_chunk->data[chunk_pos]) return;
	cursor = *(unsigned long long*)(program + cursor - sizeof(unsigned long long) + 1) - 1;
	bf_validate_cursor();
}

inline void bf_cycle_stop() {
	cursor += sizeof(unsigned long long);
	bf_validate_cursor();
	if (!current_chunk->data[chunk_pos]) return;
	cursor = *(unsigned long long*)(program + cursor - sizeof(unsigned long long) + 1) - 1;
	bf_validate_cursor();
}

inline void bf_toggle_read() {
	if (print_mode != 1) {
		print_mode = 1;
		print_pos = 0;
	} else {
		filename_buffer[print_pos] = '\0';
		freopen((char*)filename_buffer, "r", stdin);
		print_mode = print_pos = 0;
	}
}

inline void bf_toggle_write() {
	if (print_mode != 2) {
		print_mode = 2;
		print_pos = 0;
	} else {
		filename_buffer[print_pos] = '\0';
		freopen((char*)filename_buffer, "w", stdout);
		print_mode = print_pos = 0;
	}
}

#ifdef BF_DEBUG
void bf_crash() {
	int i;
	freopen("crash.log", "w", stderr);
	fprintf(stderr, "Current chunk contents:\n");
	for (i = 0; i < BF_CHUNK_SIZE; ++i) {
		fprintf(stderr, "[%05d]: %c (ASCII=%d)", i, current_chunk->data[i], (int)current_chunk->data[i]);
		if (i == chunk_pos) fprintf(stderr, " <--");
		fprintf(stderr, "\n");
	}
}
#endif

int main(int argc, char* argv[]) {
	bf_init();
	for (cursor = 0; cursor < PROGRAM_LEN; ++cursor)
		switch(program[cursor]) {
		case '>':
			bf_right();
			break;
		case '<':
			bf_left();
			break;
		case '+':
			bf_inc();
			break;
		case '-':
			bf_dec();
			break;
		case '.':
			bf_print();
			break;
		case ',':
			bf_read();
			break;
		case '[':
			bf_cycle_start();
			break;
		case ']':
			bf_cycle_stop();
			break;
		case '/':
			bf_toggle_write();
			break;
		case '\\':
			bf_toggle_read();
			break;
#ifdef BF_DEBUG
		case '!':
			bf_crash();
			return BF_ERRCODE_RUNTIME_ASSERT;
#endif
		}
	return 0;
}
