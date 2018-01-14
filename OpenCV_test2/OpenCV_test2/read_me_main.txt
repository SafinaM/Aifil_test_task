1. Была исправлена ошибка в формировании последовательности черных кадров, а также изменен исходный xml-файл.

Программа для нахождения мест склейки видео-файлов. 
1. Прилагается xml-файл my_histogram_file_temp.xml с цветовыми гистограмами кадров - образцов.
2. Результаты выводятся на экран и сохраняются в файл "results.txt".
3. Можно корректировать поведение алгоритма добавлением образцов при просмотре видео, по умолчанию кадры записываются в последовательность черных кадров. Опция требует доработки (для выбора нужной последовательсти вручную после сохранения нужного кадра).
4. Можно сохранить все изменения в файл xml. По умолчанию все изменения, сделанные во время просмотра видео не сохранятся, для этого надо запустить функцию saveNewDataForBasicFrames().
5. Видео надо поместить в корневую папку. Для выбора пути, необходимо доработать нужную опцию. 
6. В отдельном окне показывается гистограмма текущего видео.
7. Для быстрого оценивания работы алгоритма можно перемотать видео вперед.
8. Алгоритм работает на основе цветовых гистограмм.
9. Единичные срабатывания на отдельных кадрах видео не учитываются.

Program for finding borders in video file. 
The xml-file is adding to the program. The results are showing on the screen and saving to the computer in file "results.txt". 
It is possible to teach program to recognize necessary template frames on borders. 
By default the template frames are adding to "black" sequence. 
It is possible to save changes in templates in xml-file. By default changing will not happend.
Результы работы алгоритма приведены ниже.
Для первого видео: 
Frame numbers 1) 1 - 62
Frame numbers 2) 3638 - 3839
Frame numbers 3) 7390 - 7625
Frame numbers 4) 11196 - 11447
Frame numbers 5) 14882 - 15139
Frame numbers 6) 18691 - 18906
Для второго: 
Frame numbers 1) 1 - 81
Frame numbers 2) 3641 - 3836
Frame numbers 3) 7407 - 7620
Frame numbers 4) 11195 - 11452
Frame numbers 5) 14993 - 15287
Frame numbers 6) 18835 - 19101
Для третьего:
Frame numbers 1) 1 - 81
Frame numbers 2) 3641 - 3836
Frame numbers 3) 7407 - 7620
Frame numbers 4) 11195 - 11452
Frame numbers 5) 14993 - 15287
Frame numbers 6) 18835 - 19101

