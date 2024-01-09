# Bash Utils

Разработка утилит Bash по работе с текстом: cat, grep.

## Contents

-  [Part 1](#part-1-работа-с-утилитой-cat)  
-  [Part 2](#part-2-работа-с-утилитой-grep)  


### cat Использование

Cat - одна из наиболее часто используемых команд в Unix-подобных операционных системах. Команда имеет три взаимосвязанные функции в отношении текстовых файлов: отображение, объединение их копий и создание новых.

`cat [OPTION] [FILE]...`

### cat Опции

| № | Опции | Описание |
| ------ | ------ | ------ |
| 1 | -b (GNU: --number-nonblank) | нумерует только непустые строки |
| 2 | -e предполагает и -v (GNU only: -E то же самое, но без применения -v) | также отображает символы конца строки как $  |
| 3 | -n (GNU: --number) | нумерует все выходные строки |
| 4 | -s (GNU: --squeeze-blank) | сжимает несколько смежных пустых строк |
| 5 | -t предполагает и -v (GNU: -T то же самое, но без применения -v) | также отображает табы как ^I |

</br>

### grep Использование

`grep [options] template [file_name]`

### grep Опции

| № | Опции | Описание |
| ------ | ------ | ------ |
| 1 | -e | Шаблон |
| 2 | -i | Игнорирует различия регистра.  |
| 3 | -v | Инвертирует смысл поиска соответствий. |
| 4 | -c | Выводит только количество совпадающих строк. |
| 5 | -l | Выводит только совпадающие файлы.  |
| 6 | -n | Предваряет каждую строку вывода номером строки из файла ввода. |
| 7 | -h | Выводит совпадающие строки, не предваряя их именами файлов. |
| 8 | -s | Подавляет сообщения об ошибках о несуществующих или нечитаемых файлах. |
| 9 | -f file | Получает регулярные выражения из файла. |
| 10 | -o | Печатает только совпадающие (непустые) части совпавшей строки. |




- Программы разработаны на языке Си с использованием компилятора gcc 
- Код программ cat и grep папках src/cat/ и src/grep/ соответственно  
- Программы представляют собой исполняемый файл с аргументами командной строки
- Сборка программ настроена с помощью Makefile с соответствующими целями: s21_cat, s21_grep   
- Предусмтрено покрытие интеграционными тестами для всех вариантов флагов и входных значений, на базе сравнения с поведением реальных утилит Bash
- Общие модули вынесены в отдельную папку src/common
- Параметры форматирования - в src


## Part 1. Работа с утилитой cat

- Реализована поддержка всех флагов (включая GNU версии), указанных [выше](#cat-опции)

## Part 2. Работа с утилитой grep

- Реализована поддержка следующих флагов: `-e`, `-i`, `-v`, `-c`, `-l`, `-n`, `-h`, `-s`, `-f`, `-o`
- Реализована поддержка комбинаций флагов
- Для регулярных выражений использована библиотека regex  
- Исходные, заголовочные и make файлы должны располагаться в директории src/grep/
- Итоговый исполняемый файл должен располагаться в директории src/grep/ и называться s21_grep