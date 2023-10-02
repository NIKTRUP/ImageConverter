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
<img src="https://github.com/NTG-TPL/ImageConverter/assets/72292425/2e71d01d-d901-4698-8ea8-605c3a387c32" width=65% height=65%>

* Повысить яркость изображения.\
Пример: `./imgconv cat.ppm cat.jpg brightness 50`
<img src="https://github.com/NTG-TPL/ImageConverter/assets/72292425/64069301-3838-4cec-97b6-811ab7cdb2e5" width=65% height=65%>


* Понизить яркость изображения.\
Пример: `./imgconv cat.ppm cat.jpg brightness -50`
<img src="https://github.com/NTG-TPL/ImageConverter/assets/72292425/7d57753c-d67a-455c-8acd-5db09abd8a48" width=65% height=65%>

* Обрезать.\
Пример: `./imgconv cat.jpg cat_crop.jpg crop 450 150 1251 900`. Первые 2 числа после команды `crop` задают верхний левый угол в пикселях, вторые два числа - правый нижний угол.
Если размеры задаваемого поля выходят за границы изображения, то выбрасывается ошибка.
<img src="https://github.com/NTG-TPL/ImageConverter/assets/72292425/2ca7c96d-ab3b-405d-a47c-2b557ecedf48" width=65% height=65%>


* Шифрование изображения.\
Пример: `./imgconv cat.jpg cat_cypher.jpg cypher 42`. Целое положительное число после команды `cypher` задаёт ключ шифрования.
Расшифровка ранее зашифрованного изображения производится повторным вызовом программы с тем же ключом шифрования.
<img src="https://github.com/NTG-TPL/ImageConverter/assets/72292425/70a8a138-39f8-4f2d-9241-65cf4e8ca073" width=65% height=65%>


* Отражение по горизонтали.\
Пример: `./imgconv cat.jpg cat_hmirror.jpg hmirror`
<img src="https://github.com/NTG-TPL/ImageConverter/assets/72292425/b13f4751-51a6-4d18-b761-0fe5c3f76294" width=65% height=65%>

* Отражение по вертикали.\
Пример: `./imgconv cat.jpg cat_vmirror.jpg vmirror`
<img src="https://github.com/NTG-TPL/ImageConverter/assets/72292425/9e0cba5f-db69-49c7-9f2e-5b82034f444c" width=65% height=65%>

* Создание негатива.\
Пример: `./imgconv cat.jpg cat_negate.jpg negate`
<img src="https://github.com/NTG-TPL/ImageConverter/assets/72292425/dcf9e2e8-59df-42b3-bad6-9288c68aad47" width=65% height=65%>

* Применение оператора Собеля.\
Пример: `./imgconv cat.jpg cat_sobel.jpg sobel`
<img src="https://github.com/NTG-TPL/ImageConverter/assets/72292425/069bb93d-5145-4844-80e2-6b72d2e2dd23" width=65% height=65%>

# Сборка и установка
Для сборки программы необходим компилятор С++, поддерживающий стандарт не ниже С++17, CMake не ниже версии 3.11. , библиотека LibJPEG
  > ## Сборка с помощью CMake
  > 1.	Для поддержки файлов jpeg используется библиотека LibJPEG `https://www.ijg.org/`.
  В папке libjpeg собрана библиотека под Linux (Ubuntu 22.04),
  > 2.	Создайте папку `build` для сборки программы.
  > 3.	Перейдите в `build` и введите в терминал команду `cmake ../ImgConverter -DCMAKE_BUILD_TYPE=Debug -DLIBJPEG_DIR=../libjpeg`
  (Если ваша конфигурация файлов отличается, то общий вид команды `cmake <Путь CMakeLists.txt ImgConverter> -DCMAKE_BUILD_TYPE=<Тип сборки> -DLIBJPEG_DIR=<Путь к собранной библиотеке libjpeg>`).
  > 5.	Введите команду : `make` или `make -j<кол-во ядер процессора>` для ускорения процесса сборки 
  > 6.	После успешной сборки в каталоге `build` появится исполняемый файл `imgconv` (`imgconv.exe` в Windows)


