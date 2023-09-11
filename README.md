# ImageConverter - Редактор изображений
## Возможности
* Конвертирование изображения из одного типа в другой. Поддерживаются `ppm`, `jpeg`, `bmp`.
* Отражение по вертикали `vmirror`
* Отражение по горизонтали `hmirror`
* Создание негатива `negate`
* Изменение яркости `brightness <value>`
* Обрезка `crop <lt_x> <lt_y> <rd_x> <rd_y>`
* Обработка оператором Собеля `sobel`
* Шифрование изображения `cypher <key>`

## Использование собранной версии программы
Программа запускается в консоли с ключами:\
`./imgconv <путь входного изображения> <путь выходного изображения> [hmirror|vmirror|sobel|negate|cypher <key>|brightness <value>|crop <lt_x> <lt_y> <rd_x> <rd_y>]`

* Конвертирование изображения из одного типа в другой. Поддержка `ppm`, `jpeg`, `bmp`.\
Пример: `./imgconv cat.ppm cat.jpg`
<img src="https://github.com/NIKTRUP/ImageLib/assets/72292425/c584d77b-d75c-4832-b8ea-e83ae97080fe" width=50% height=50%>


* Повысить яркость изображения.\
Пример: `./imgconv cat.ppm cat.jpg brightness 50`
<img src="https://github.com/NIKTRUP/ImageLib/assets/72292425/776ff205-ca7c-455f-ba07-4b42c53a1bb3" width=50% height=50%>


* Понизить яркость изображения.\
Пример: `./imgconv cat.ppm cat.jpg brightness -50`
<img src="https://github.com/NIKTRUP/ImageLib/assets/72292425/d10ea50b-b68d-49a1-9662-9242f4b2fc75" width=50% height=50%>


* Обрезать.\
Пример: `./imgconv cat.jpg cat_crop.jpg crop 450 150 1251 900`. Первые 2 числа после команды `crop` задают верхний левый угол в пикселях, вторые два числа - правый нижний угол.
Если размеры задаваемого поля выходят за границы изображения, то выбрасывается ошибка.
<img src="https://github.com/NIKTRUP/ImageLib/assets/72292425/169bbb34-5323-4f3b-989c-32747fea2e16" width=50% height=50%>


* Шифрование изображения.\
Пример: `./imgconv cat.jpg cat_cypher.jpg cypher 42`. Целое положительное число после команды `cypher` задаёт ключ шифрования.
Расшифровка ранее зашифрованного изображения производится повторным вызовом программы с тем же ключом шифрования.
<img src="https://github.com/NIKTRUP/ImageLib/assets/72292425/60b3d873-6d6e-4f2d-972a-262eae8c6b0b" width=50% height=50%>


* Отражение по горизонтали.\
Пример: `./imgconv cat.jpg cat_hmirror.jpg hmirror`
<img src="https://github.com/NIKTRUP/ImageLib/assets/72292425/a37d8a46-2cef-45ec-b71d-888c32b379c4" width=50% height=50%>


* Отражение по вертикали.\
Пример: `./imgconv cat.jpg cat_vmirror.jpg vmirror`
<img src="https://github.com/NIKTRUP/ImageLib/assets/72292425/6277e987-9c63-486e-88dc-804ea269355c" width=50% height=50%>


* Создание негатива.\
Пример: `./imgconv cat.jpg cat_negate.jpg negate`
<img src="https://github.com/NIKTRUP/ImageLib/assets/72292425/fd98d6f4-2c85-4b93-bbdb-a87b1a54bc2c" width=50% height=50%>


* Применение фильтра Собеля.\
Пример: `./imgconv cat.jpg cat_sobel.jpg sobel`
<img src="https://github.com/NIKTRUP/ImageLib/assets/72292425/8e00dfcf-ec52-4134-9e60-ffc3be52c889" width=50% height=50%>


> ## Сборка с помощью CMake
> 1.	Для поддержки файлов jpeg используется библиотека LibJPEG `https://www.ijg.org/`.
В папке libjpeg собрана библиотека под Linux (Ubuntu 22.04),
> 2.	Создайте папку `build` для сборки программы.
> 3.	Перейдите в `build` и введите в терминал команду `cmake ../ImgConverter -DCMAKE_BUILD_TYPE=Debug -DLIBJPEG_DIR=../libjpeg`
(Если ваша конфигурация файлов отличается, то общий вид команды `cmake <Путь CMakeLists.txt ImgConverter> -DCMAKE_BUILD_TYPE=<Тип сборки> -DLIBJPEG_DIR=<Путь к собранной библиотеке libjpeg>`).
> 5.	Введите команду : `make` или `make -j<кол-во ядер процессора>` для ускорения процесса сборки 
> 6.	После успешной сборки в каталоге `build` появится исполняемый файл `imgconv` (`imgconv.exe` в Windows)
