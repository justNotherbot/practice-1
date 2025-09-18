#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>

const char* INVARG = "Invalid argument format\n";
const char* FFLAG = "--file";
const char *WFLAG = "--word";

bool issmol(char c)
{
  return c >= 'a' && c <= 'z';
}

void updmsk(char *warg, int *msk)
{
  int i = 0;
  while(warg[i] != '\0')
  {
    if(issmol(warg[i]))
      msk[warg[i]-'a'] = 1;
    i++;
  }
}

int main(int argc, char** argv) {
  char* word_param = nullptr;
  char* file_param = nullptr;
  if (argc != 5) {
    puts(INVARG);
    return 0;
  } else if (!strcmp(argv[1], FFLAG) && !strcmp(argv[3], WFLAG)) {
    file_param = argv[2];
    word_param = argv[4];
  } else if (!strcmp(argv[3], FFLAG) && !strcmp(argv[1], WFLAG)) {
    file_param = argv[4];
    word_param = argv[2];
  } else {
    puts(INVARG);
    return 0;
  }

  int msk[26];
  for(int i = 0; i < 26; i++)
    msk[i] = 0;
  updmsk(word_param, msk);

  int is_ok = EXIT_FAILURE;
  FILE* fp = std::fopen(file_param, "r");
  if (!fp) {
    std::perror("File opening failed");
    return is_ok;
  }

  char c;  // Note: int, not char, required to handle EOF
  char cr[2];
  cr[1] = '\0';
  int ans = 0;
  bool onw = 0;
  bool crw = 0;
  while ((cr[0] = std::fgetc(fp)) != EOF)  // Standard C I/O file reading loop
  {
    if(!issmol(cr[0]))
    {
      ans += crw;
      crw = 0;
      onw = 0;
    }
    else
    {
      if(!onw)
      {
        onw = 1;
        if(msk[cr[0]-'a'])
        {
          crw = 1;
        }
      }
      else if(!msk[cr[0]-'a'])
      {
        crw = 0;
      }
    }
  }

  if (std::ferror(fp))
  {
    std::puts("I/O error when reading");
  }
  else if (std::feof(fp)) {
    printf("Word count is %d\n", ans);
  }
    
  
  

  std::fclose(fp);

  return 0;
}
