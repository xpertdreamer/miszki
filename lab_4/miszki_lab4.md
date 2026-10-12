## Создание и настройка ВМ

1. Осуществил создание двух виртуальных машин на базе ОС Windows и Ubuntu Linux, для проверки сделал `ping` с двух сторон, а затем создал клоны базовых ВМ.

<img src="./img/pingwtou.png" alt="ping1" width="800">

<img src="./img/pingutow.png" alt="ping2" width="800">

## Часть 1. Подключение по ssh

2. На машине под управлением Ubuntu установил необходимый софт для развертывания ssh сервера

<img src="./img/opensshserverinstallation.png" alt="openssh-server installation" width="800">

3. Проверил статус фаервола, чтобы он не ограничил подлючение в дальнейшем, добавил нового пользователя, к которому будет происходить подключение

<img src="./img/5-addsshuser.png" alt="check" width="800">

4. Осуществил первое подключение с машины Windows на машину Ubuntu по паролю

<img src="./img/6-sshfirstconnection.png" alt="ssh-first-connection" width="800">

5. Сгенерировал ssh ключ на машине Windows с использованием команды ``ssh-keygen -t ed25519`` и передал его на на машину под управлением Ubuntu

<img src="./img/7-keytransport.png" alt="key-transport" width="800">

6. Попробовал осуществить подключение по переданному ключу

<img src="./img/8-keylogin.png" alt="key-login" width="800">

7. Запретил вход по паролю при подключении по ssh

<img src="./img/9-nopassword.png" alt="no-password" width="800">

8. Проверил текущего пользователя и его права 

<img src="./img/10-sshuserprevs.png" alt="check-user" width="800">

9. Остановил службу ssh и поменял порт на `222` вместо `22`

<img src="./img/11-changeport222.png" alt="change-port" width="800">

10. Попробовал зайти на сервер с новым портом

<img src="./img/12-newportlogin.png" alt="new-login" width="800">

11. Вывел лог сервера ``sudo journalctl -u ssh -n 50``

<img src="./img/13-sshlogs.png" alt="logs" width="800">

## Часть 2. Работа с xrdp

12. Сделал новые клоны базовых ВМ. На машине под управлением Ubuntu для нормальной работы перешел c wayland на xorg (x-server)

<img src="./img/14-xorgmigration.png" alt="xorg-migration-1" width="800">

<img src="./img/14-xorgmigration2.png" alt="xorg-migration-2" width="800">

13. Установил xrdp

<img src="./img/15-xrdpinstallation.png" alt="xrdp-installation" width="800">

14. Добавил пользователя в необходимые группы

<img src="./img/16-xrdpaddtogroup.png" alt="xrdp-add-group" width="800">

15. Осуществил настройку xrdp сессии. Будет использоваться DE GNOME, так как он уже установлен и не требует доп. настройки

<img src="./img/17-xrdpsessionconf.png" alt="xrdp-session-conf" width="800">

16. Подключился по протоколу rdp к настроенному серверу с машины Windows 

<img src="./img/18-xrdpconnection.png" alt="xrdp-connection" width="800">

17. Проверил подключение по xrdp

<img src="./img/19-xrdpsession.png" alt="xrdp-check" width="800">
