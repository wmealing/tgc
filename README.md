# tgc
A simple to use thread safe library to interact with the Telegram bot API.

## Install
Before being able to compile you'll need libcurl and jansson. On Debian based distros this can be done like so:
```bash
apt install libcurl-dev libjansson-dev
```

Afterwards to clone and compile execute the following commands:
```bash
git clone https://github.com/77616c6964/tgc.git
cd tgc/src
make
sudo make install
```


## Example

```c
#include <tgapi.h>

int main ()
{
  User_s bot_info;
  tg_res res;
  char *token = "123456789:AaBbCcDdEeFf";

  tg_init (token, &res);
  bot_info = getMe (&res);

  printf ("Hello, I'm %s.\n", bot_info.first_name);

  User_clean (bot_info);

  return 0;
}
```

## License 
This library is licensed under the [GPL-3.0](https://opensource.org/licenses/GPL-3.0)
