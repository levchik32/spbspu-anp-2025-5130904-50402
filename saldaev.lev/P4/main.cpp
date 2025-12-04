#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <limits>

namespace saldaev
{
  const size_t block_size = 10;

  char *getLine(std::istream &in, const size_t block_size)
  {
    char *data = reinterpret_cast<char *>(malloc((block_size + 1) * sizeof(char)));
    if (data == nullptr)
    {
      return nullptr;
    }
    bool skipws_on = in.flags() & std::ios_base::skipws;
    if (skipws_on)
    {
      in >> std::noskipws;
    }

    size_t capacity = block_size;
    size_t size = 0;
    char crnt_char = ' ';
    while (in >> crnt_char && crnt_char != '\r' && crnt_char != '\n')
    {
      size++;
      if (size > capacity)
      {
        char *tmp = reinterpret_cast<char *>(malloc((capacity + block_size + 1) * sizeof(char)));
        if (tmp == nullptr)
        {
          free(data);
          if (skipws_on)
          {
            in >> std::skipws;
          }
          return nullptr;
        }
        capacity += block_size;
        for (size_t i = 0; i < size - 1; ++i)
        {
          tmp[i] = data[i];
        }
        free(data);
        data = tmp;
      }
      data[size - 1] = crnt_char;
    }
    if (skipws_on)
    {
      in >> std::skipws;
    }
    if (crnt_char == '\r' || crnt_char == '\n' || in.eof())
    {
      data[size] = '\0';
      return data;
    }
    free(data);
    return nullptr;
  }

  char *createCompactArray1(const char *data)
  {
    if (data == nullptr)
    {
      return nullptr;
    }
    char prev = ' ';
    char crnt = ' ';
    size_t leters = 0;
    size_t spaces = 0;
    size_t i = 0;
    crnt = data[i];
    while (crnt != '\0')
    {
      if (crnt != ' ')
      {
        leters++;
      }
      else if (prev != ' ' && crnt == ' ')
      {
        spaces++;
      }
      prev = crnt;
      i++;
      crnt = data[i];
    }
    if (leters && prev == ' ')
    {
      spaces--;
    }
    char *new_array = reinterpret_cast<char *>(malloc((leters + spaces + 1) * sizeof(char)));
    if (new_array == nullptr)
    {
      return nullptr;
    }
    new_array[leters + spaces] = '\0';
    return new_array;
  }

  char *createCompactArray2(const char *data)
  {
    size_t new_length = 1;
    size_t i = 0;
    char crnt = data[i];
    while (crnt != '\0')
    {
      if (!std::isalpha(crnt))
      {
        new_length++;
      }
      i++;
      crnt = data[i];
    }
    char *new_array = reinterpret_cast<char *>(malloc(new_length * sizeof(char)));
    if (new_array == nullptr)
    {
      return nullptr;
    }
    return new_array;
  }

  char * spcRmv(const char *data)
  {
    char * new_arr = createCompactArray1(data);
    if (new_arr == nullptr)
    {
      return 0;
    }
    size_t crnt_digit = 0;
    size_t i = 0;
    char crnt_char = data[i];
    char prev_char = ' ';
    while (crnt_char != '\0')
    {
      if (crnt_char != ' ' || prev_char != ' ')
      {
        new_arr[crnt_digit] = crnt_char;
        crnt_digit++;
      }
      prev_char = crnt_char;
      i++;
      crnt_char = data[i];
    }
    return new_arr;
  }
  char * latRmv(char *data)
  {
    char * new_arr = createCompactArray2(data);
    if (new_arr == 0)
    {
      return 0;
    }
    size_t crnt_digit = 0;
    size_t i = 0;
    char crnt_char = data[i];
    while (crnt_char != '\0')
    {
      if (!isalpha(crnt_char))
      {
        new_arr[crnt_digit] = crnt_char;
        crnt_digit++;
      }
      i++;
      crnt_char = data[i];
    }
    return new_arr;
  }
}

int main()
{
  char *line = nullptr;
  line = saldaev::getLine(std::cin, saldaev::block_size);
  if (line == nullptr)
  {
    std::cerr << "Could not read the string\n";
    return 1;
  }

  char *new_line = nullptr;
  new_line = saldaev::spcRmv(line);
  if (new_line == nullptr)
  {
    std::cerr << "Could not convert the string\n";
    free(line);
    return 1;
  }
  std::cout << new_line << "\n";
  free(new_line);

  new_line = nullptr;
  new_line = saldaev::latRmv(line);
  if (new_line == nullptr)
  {
    std::cerr << "Could not convert the string\n";
    free(line);
    return 1;
  }
  std::cout << new_line << "\n";
  free(line);
  free(new_line);
}
