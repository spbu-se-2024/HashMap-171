# HashMap-171

Проект по разработке hash-таблиц 171 группы.


## Сборка и запуск проекта

```bash
# Сборка
mkdir build
cd build
cmake ..
make

# Запуск
cd ..
build/src/main/run_app <FILE> <SIZE> <HASH-FUNC>
```

| Параметр    | Описание                                                              |
|:------------|:----------------------------------------------------------------------|
| *FILE*      | *файл*, например `src/res/war_and_peace.txt` (доступен в репозитории) |
| *SIZE*      | *размер хеш-таблицы*, **должен быть больше 0**                        |
| *HASH-FUNC* | *хеш-функция*, одна из **md5**, **polynomial**, **sha-1**             |


## Необходимые ссылки (для разработчитков):

1) **Clion Settings** (*инструкция ниже*): https://github.com/spbu-se-2024/group-171-idea-settings.git
2) https://docs.google.com/document/d/137yjpAnTCETFIGQkWUe0j8b80ubKzs8bIWOzOciRRKg/edit?ts=5fb7fc53 - гугл документ с обсуждениями
3) В папке [interface-suggestions](https://github.com/spbu-se-2024/HashMap-171/tree/master/interface-suggestions) находятся предложения по поводу интерфейса
4) Markdown - ресурсы:
   - https://markdown-it.github.io/
   - https://daringfireball.net/projects/markdown/
5) [Инструкция по написанию тестов.](https://github.com/spbu-se-2024/HashMap-171/blob/master/src/test/CUTEST_GUIDE.md)


### Clion Settings

File -> Manage IDE Settings -> Settings Repository -> Вставьте ссылку (https://github.com/spbu-se-2024/group-171-idea-settings.git) -> Overwrite Local
