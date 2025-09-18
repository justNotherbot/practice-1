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

void updmsk(char *warg, int *mskl, int *msku)
{
  int i = 0;
  while(warg[i] != '\0')
  {
    if(issmol(warg[i]))
      mskl[warg[i]-'a'] = 1;
    else if(isbig(warg[i]))
      msku[warg[i]-'A'] = 1;
    i++;
  }
}

bool inmsk(char c, int *mskl, int *msku)
{
  if(issmol(c))
    return mskl[c-'a'];
  if(isbig(c))
    return msku[c-'A'];
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

  int mskl[26];
  int msku[26];
  int crl[26];
  int cru[26];
  for(int i = 0; i < 26; i++)
  {
    mskl[i] = 0;
    msku[i] = 0;
    crl[i] = 0;
    cru[i] = 0;
  }
    
  updmsk(word_param, mskl, msku);

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

  while ((cr[0] = std::fgetc(fp)) != EOF)  // Standard C I/O file reading loop
  {
    if(issmol(cr[0]))
    {
      crl[cr[0]-'a'] = 1;
    }
    else if(isbig(cr[0]))
    {
      cru[cr[0]-'A'] = 1;
    }
    if(!iscompat(cr[0]))
    {
      for(int i = 0; i < 26; i++)
      {
        if(msku[i] != cru[i] || crl[i] != mskl[i])
          crw = 0;
        cru[i] = 0;
        crl[i] = 0;
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
        
        if(inmsk(cr[0], mskl, msku))
        {
          crw = 1;
        }
      }
      else if(!inmsk(cr[0], mskl, msku))
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
