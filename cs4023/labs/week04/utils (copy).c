char flipChar(char c)
{
  if('a'<=c&&c<='z')
    return c='z'-(c-'a');
  else if ('A'<=c&&c<='Z')
  return c='Z'-(c-'A');
  else if ('0'<=c&&c<='9')
    return c='9'-(c-'0');
  else
    return c;
}
