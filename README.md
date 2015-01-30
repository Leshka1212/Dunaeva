Спектрограмма

Программа строит спектрограмму и параллельно воспроизводит аудио файл. Основным алгоритмом в программе служит быстрое преобразование Фурье.
Программа написана на языке с++ с использованием библеотеки Qt. В программе за основу взяты классы М. Л. Мячина.

Для запуска программы требуется NetBeans с установленной библеотекой Qt. Программа тестировалась на Windows 7, 8, 8.1.

Программа представляет собой оконное приложение. Вид программы:
1. Меню выбора файла.
2. Управление воспроизведением музыки.
3. Окно, в котором рисуется спектрограмма.

Программа поддерживает аудиофайлы в формате .wav. Для того, чтобы выбрать файл, нужно в меню нажать Файл->Открыть файл, далее указать путь к файлу и нажать Открыть.
Файл моментально начнет проигрываться и параллельно будет рисоваться спестрограмма. Чтобы остановить воспроизведение музыки, нужно нажать Stop, а, чтобы восстановить воспроизведение, нужно нажать Play.
В окне Спектрограмма есть две шкалы:
1. Вертикальная - частота, где более высокие значения располагаются ниже.
2. Горизонтальная - время.
За наличие той или иной частоты отвечает раскрашивание в цвет, а за амплитуду - толщина линий.

Принцип работы программы заключается в следующем:
1. Считываем по 1024 отсчета из искомого файла, записываем в память.
2. Прореживаем сигнал.
3. Применяем оконную функцию.
4. Применяем БПФ.
5. Параллельно рисуем спектрограмму и воспроспроизводим музыку, используя таймер.
6. Повторяем шаги 1-5 до того момента, как весь файл не будет считан.

Участники
1. Бовин Андрей ИВТ-21 МО
реализация основных алгоритмов, воспроизведение аудиофайла
2. Оганесян Мурат ИВТ-21 МО
реализация фильтра низких частот, разбиение по классам, главная форма
3. Малков Алексей ИВТ-21 МО
организационные моменты и основу программы
4. Шестаков Денис ИВТ-21 МО
визуализация