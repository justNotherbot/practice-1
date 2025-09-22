#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

const char* kInvArg = "Invalid argument format";
const char* kFileSign = "--file";
const char* kWordSign = "--word";
constexpr int kAlphSize = 26;
constexpr int kReqArgc = 5;

bool IsLower(char c) {
  return c >= 'a' && c <= 'z'; 
}

bool IsUpper(char c) { 
  return c >= 'A' && c <= 'Z'; 
}

bool IsDigit(char c) { 
  return c >= '0' && c <= '9'; 
}

bool IsWordSymbol(char c) { 
  return IsDigit(c) || IsLower(c) || IsUpper(c); 
}

void UpdateMask(char* filter_word, int* mask) {
  int i = 0;
  while (filter_word[i] != '\0') {
    if (IsLower(filter_word[i]))
      mask[filter_word[i] - 'a'] = 1;
    else if (IsUpper(filter_word[i]))
      mask[filter_word[i] - 'A'] = 1;
    i++;
  }
}

bool CheckInMask(char c, int* mask) {
  if (IsLower(c)) {
    return mask[c - 'a'];
  }
  if (IsUpper(c)) {
    return mask[c - 'A'];
  }
  return 0;
}

void ParseCmdParams(int argc, char** argv, char** word_param_ptr, char** file_param_ptr){
  assert(argc == kReqArgc);
  if (!strcmp(argv[1], kFileSign) && !strcmp(argv[3], kWordSign)) {
    *file_param_ptr = argv[2];
    *word_param_ptr = argv[4];
  } 
  else if (!strcmp(argv[3], kFileSign) && !strcmp(argv[1], kWordSign)) {
    *file_param_ptr = argv[4];
    *word_param_ptr = argv[2];
  }
}

int main(int argc, char** argv) {
  char* word_param = nullptr;
  char* file_param = nullptr;
  if (argc != kReqArgc) {
    puts(kInvArg);
    return 1;
  } 
  else {
    ParseCmdParams(argc, argv, &word_param, &file_param);
  }
  if(word_param == nullptr || file_param == nullptr) {
    puts(kInvArg);
    return 1;
  }

  int mask[kAlphSize];
  int curr_mask[kAlphSize];
  for (int i = 0; i < kAlphSize; i++) {
    mask[i] = 0;
    curr_mask[i] = 0;
  }

  UpdateMask(word_param, mask);

  int file_status = EXIT_FAILURE;
  FILE* file_stream = fopen(file_param, "r");
  if (!file_stream) {
    perror("File opening failed");
    return file_status;
  }

  char curr_symbol[2];
  curr_symbol[1] = '\0';
  int n_suit_words = 0;
  while (true) {
    curr_symbol[0] = fgetc(file_stream);

    if (!IsWordSymbol(curr_symbol[0])) {
      bool curr_word_ok = true;
      for (int i = 0; i < kAlphSize; i++) {
        if (mask[i] == 1 && curr_mask[i] == 0) {
          curr_word_ok = false;
        }
        curr_mask[i] = 0;
      }
      n_suit_words += curr_word_ok;
    } 
    else {
      if (IsLower(curr_symbol[0])) {
        curr_mask[curr_symbol[0] - 'a'] = 1;
      } 
      else if (IsUpper(curr_symbol[0])) {
        curr_mask[curr_symbol[0] - 'A'] = 1;
      }
    }
    if (curr_symbol[0] == EOF) {
      break;
    }
  }
  if (ferror(file_stream)) {
    puts("I/O error when reading");
  } 
  else if (feof(file_stream)) {
    printf("Word count is %d\n", n_suit_words);
  }
  fclose(file_stream);

  return 0;
}
