# cyberNet

Исходный код трех робототехнических проектов, созданных участниками летней проектной смены 2016 в образовательном центре Сириус.

##1. Робот "Старший брат" (BigBrother)

Робот умеет общаться с пользователем с помощью микрофонов и динамика, перемещаться на своей омни-платформе, отслеживать предметы и роботов-помощников при помощи камеры, хватать предметы своим манипулятором. Также в него был встроен блок для выбрасывания визиток.
На роботе установлены 2 контроллера ТРИК, которые связаны по Wi-Fi. 
Директория проекта - BigBrother.
Основная программа для контроллера, отвечающего за общение, <b>speak_work.js</b>, а для второго (перемещение и движения глаз) - <b>eyes.js</b>.
Также выложены промежуточные результаты с отельными функцими для робота: <b>startHelper.js</b>, <b>reg.js</b>, <b>money.js</b>.

##2. Мозг робота "Старшего брата" (brainBigBrother)

Программа перехватывает видеотрансляцию с робота "Старшего брата" и по его команде отслеживает робота помощника, что висит на школьной маркерной доске. Программа детектирует области обведенные в красный и засылает туда робота помощника. В случае, если на доске не осталось зон для очистки, робот помощник возвращается в угол доски.

##3. Робот помощник (Helper)

Программа управления движением робота на доске. Робот получает команды от мозга "Старшего брата" о дальнейшем движении и исполняет их.

##4. Автоматическая парковка 

<b>parking.js</b> - программа для автомобиля, который взаимодействует с сервером парковки<br>
<b>control.js</b> - программа парковочной системы, которая взаимодействует с сервером и контролирует шлагбаум<br>
<b>brainParking</b> - программа для сервера с обработкой видетрансляции с парковки. Определяет свободные парковочные места. По запросу автомобиля выдает ближайшее свободное парковочное место, оповещая при этом парковочную систему. 

##5. Робот уборщик

Чистит заданную прямоугольную зону, после чего возвращается на базу. Поиск базы происходит с помощью установленного видеомодуля: робот крутится и ищет желтую прямоугольную метку, после чего доезжает до нее и останавливается.
