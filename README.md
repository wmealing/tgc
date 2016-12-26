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
cd tgc/
make
sudo make install
```

```
If you're using cmake to build this project in a cross platform manner

git clone https://github.com/77616c6964/tgc.git
cd tgc
mkdir build && cd build
cmake install ..
make
```


## Example

```c
#include <tgapi.h>

int main ()
{
    User_s bot_info;
    tg_res res;
    char *token = "123456789:AaBbCcDdEeFf";

    if (tg_init (token, &res))
        return 1; // Exits if the Library fails to initialize.

    bot_info = getMe (&res);

    if (res.ok == TG_OKAY && bot_info.first_name)
    {
        printf ("Hello, I'm %s.\n", bot_info.first_name);
        User_free (bot_info);
    }

    return 0;
}
```

## License 
This library is licensed under the [GPL-3.0](https://opensource.org/licenses/GPL-3.0)
