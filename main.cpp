#include <cstdio>
#include <cstdlib>
#include <cstring>

const char* INVARG = "Invalid argument format\n";
const char* FFLAG = "--file";
const char *WFLAG = "--word";

bool issmol(char c)
{
  return c >= 'a' && c <= 'z';
}

bool isbig(char c)
{
  return c >= 'A' && c <= 'Z';
}

bool isdig(char c)
{
  return c >= '0' && c <= '9';
}

bool iscompat(char c)
{
  return isdig(c) || issmol(c) || isbig(c);
}

void updmsk(char *warg, int *msk)
{
  int i = 0;
  while(warg[i] != '\0')
  {
    if(issmol(warg[i]))
      msk[warg[i]-'a'] = 1;
    else if(isbig(warg[i]))
      msk[warg[i]-'A'] = 1;
    i++;
  }
}

bool inmsk(char c, int *msk)
{
  if(issmol(c))
    return msk[c-'a'];
  if(isbig(c))
    return msk[c-'A'];
  return 0;
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
  int crmsk[26];
  for(int i = 0; i < 26; i++)
  {
    msk[i] = 0;
    crmsk[i] = 0;
  }
    
  updmsk(word_param, msk);

  int is_ok = EXIT_FAILURE;
  FILE* fp = std::fopen(file_param, "r");
  if (!fp) {
    std::perror("File opening failed");
    return is_ok;
  }

  char cr[2];
  cr[1] = '\0';
  int ans = 0;
  bool onw = 0;
  bool crw = 0;


  while (1)
  {
    cr[0] = std::fgetc(fp);
    if(issmol(cr[0]))
    {
      crmsk[cr[0]-'a'] = 1;
    }
    else if(isbig(cr[0]))
    {
      crmsk[cr[0]-'A'] = 1;
    }
    
    if(!iscompat(cr[0]))
    {
      for(int i = 0; i < 26; i++)
      {
        if(msk[i] == 1 && crmsk[i] == 0)
        {
          
          crw = 0;
        }
        crmsk[i] = 0;
      }
      ans += crw;
      crw = 0;
      onw = 0;
    }
    else
    {
      if(!onw)
      {
        onw = 1;
        
        if(inmsk(cr[0], msk))
        {
          crw = 1;
        }
      }
    }
    if(cr[0] == EOF)
      break;
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
