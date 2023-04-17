<h1 id="tcpchat">TCPChat</h1>
<p>Курсовое задание в рамках обучения в iT-школе НТЦ Протей</p>
<h1 id="-">Содержание</h1>
<ol>
<li><a href="#introduction">Введение</a>  <ol>
<li><a href="#intro1">Описание</a></li>
</ol>
</li>
<li><p><a href="#paragraph1">Структура проекта</a>  </p>
<ol>
<li><a href="#subparagraph1">Сервер</a>  <ol>
<li><a href="#sspar1">Socket</a></li>
<li><a href="#sspar2">Server</a></li>
<li><a href="#sspar3">Logger</a></li>
<li><a href="#sspar4">ChatMember</a></li>
</ol>
</li>
<li><a href="#subparagraph2">Модульные тесты</a><ol>
<li><a href="#sspar21">Logger</a></li>
<li><a href="#sspar22">ChatMember</a></li>
<li><a href="#sspar23">Socket</a></li>
</ol>
</li>
</ol>
</li>
<li><p><a href="#build">Сборка проекта</a></p>
</li>
</ol>
<p><a name="introduction"></p>
<h2 id="-">Введение</h2>
<p><a name="intro1"></p>
<h3 id="-">Описание</h3>
<p>В данной задаче нужно реализовать серверное приложение, которое взаимодействует с клиентами через tcp-соединения. Клиенты подключаются к серверу на его порт и сначала авторизуются (представляются), после чего получают доступ к текущему списку участников и потоку сообщений, а также могут отправлять свое сообщение в общий чат.
В принципе, для подключения к серверу кажется достаточным использования обычной утилиты telnet. Сервер должен для каждого нового подключения отправить строку с информацией о сервере и приглашение на ввод своего nick-name.</p>
<blockquote>
<p>Написанный сервер может одновременно обслуживать несколько подключенных клиентов (участников чата), т.к. является асинхронным.
</blockquote>
</a>
</a>
<a name="paragraph1"></p>
<h2 id="-">Структура проекта</h2>
<p>Проект состоит из:</p>
<ul>
<li>Cmake-файлов, </li>
<li>скрипта для сборки, </li>
<li>директории build,</li>
<li>основной директории Server</li>
</ul>
<p>Директория Server содержит 2 проекта (Модульные тесты <code>UnitTests</code> и сам сервер <code>Server</code>). Проект Server подразделен на каталоги с исходным кодом (<em>.cpp) <code>src</code> и заголовочными файлами </em>.h <code>include</code>.</p>
<p>При проектировании программы использовалась парадигма объектно-ориентированного программирования. В связи с этим, проект <code>Server</code> логически разбит на несколько модулей, каждый из которых представляет собой класс, описывающий определенную абстракцию (за исключением модуля main.cpp). В проекте используется библиотека Boost (Asio, log)
<a name="subparagraph1"></p>
<h3 id="-">Сервер</h3>
<p>Состоит из 4 классов
<a name="#sspar1"></p>
<h4 id="socket">Socket</h4>
<p>Класс описывает сокет подключившегося клиента, в аргумент конструктора принимает правостороннюю ссылку на объект <code>socket</code> из <code>boost::asio;</code></p>
<h6 id="-">Описание методов класса</h6>
<table>
<thead>
<tr>
<th>Метод</th>
<th>Доступ</th>
<th>Возвращаемое значение</th>
<th>Аргументы</th>
<th>Описание</th>
</tr>
</thead>
<tbody>
<tr>
<td>Start</td>
<td>public</td>
<td>void</td>
<td>message_handler&amp;&amp;, error_handler&amp;&amp;</td>
<td>Передает обработчик сообщений и обработчик ошибок, затем запускает асинхронное чтение</td>
</tr>
<tr>
<td>AsyncRead</td>
<td>private</td>
<td>void</td>
<td>-</td>
<td>Вызывает функцию асинхронного чтения async_read_until (из библиотеки Asio), считывает данные до символа <code>&#39;\n&#39;</code>, затем вызывает ReadData</td>
</tr>
<tr>
<td>ReadData</td>
<td>private</td>
<td>void</td>
<td>error_code, size_t</td>
<td>В случае отсутствия ошибок в переменной error_code, считывает данные из буфера и снова вызывает асинхронное чтение AsyncRead, иначе - закрывает сокет и выводит ошибку</td>
</tr>
<tr>
<td>AsyncWrite</td>
<td>private</td>
<td>void</td>
<td>-</td>
<td>Вызывает функцию асинхронной async_write (из библиотеки Asio), записывает первый элемент из очереди сообщений, затем вызывает WriteData</td>
</tr>
<tr>
<td>WriteData</td>
<td>private</td>
<td>void</td>
<td>error_code, size_t</td>
<td>В случае отсутствия ошибок в переменной error_code, удаляет первый элемент из очереди сообщений, в случае наличия еще сообщений в очереди - вызывает снова асинхронную запись AsyncWrite, иначе - закрывает сокет и выводит ошибку</td>
</tr>
<tr>
<td>Send</td>
<td>public</td>
<td>void</td>
<td>const std::string &amp;</td>
<td>Добавляет сообщение в очередь на отправку</td>
</tr>
<tr>
<td>GetChatMember()</td>
<td>public</td>
<td>ChatMember*</td>
<td>-</td>
<td>Геттер для приватного поля member (участник чата)</td>
</tr>
</tbody>
</table>
<p></a>
<a name="#sspar2"></p>
<h4 id="server">Server</h4>
<p>Открывает соединение. В конструктор принимает ссылку на io_context и порт.</p>
<h6 id="-">Описание методов класса</h6>
<table>
<thead>
<tr>
<th>Метод</th>
<th>Доступ</th>
<th>Возвращаемое значение</th>
<th>Аргументы</th>
<th>Описание</th>
</tr>
</thead>
<tbody>
<tr>
<td>AsyncAccept</td>
<td>public</td>
<td>void</td>
<td>-</td>
<td>Асинхронно принимает входящие подключения, добавляет клиента в связанный контейнер</td>
</tr>
<tr>
<td>Send</td>
<td>public</td>
<td>void</td>
<td>const std::string &amp;</td>
<td>Отправляет сообщения всем авторизованным участникам чата</td>
</tr>
</tbody>
</table>
<p></a>
<a name="#sspar3"></p>
<h4 id="logger">Logger</h4>
<p>Статический класс-обертка для логгера <code>boost::log</code></p>
<h6 id="-">Описание методов класса</h6>
<table>
<thead>
<tr>
<th>Метод</th>
<th>Доступ</th>
<th>Возвращаемое значение</th>
<th>Аргументы</th>
<th>Описание</th>
</tr>
</thead>
<tbody>
<tr>
<td>Start</td>
<td>public</td>
<td>void</td>
<td>-</td>
<td>Инициализирует логгер(добавляет лог-файл и настраивает его свойства)</td>
</tr>
<tr>
<td>Send</td>
<td>public</td>
<td>void</td>
<td>const tr::severity_level&amp;, const char*</td>
<td>Записывает логи. В качестве 1 аргумента принимает уровень строгости (перечисление) из пространства имен <code>boost::log::v2_mt_posix::trivial</code>, 2 аргументом принимает сообщение.</td>
</tr>
</tbody>
</table>
<p></a>
<a name="#sspar4"></p>
<h4 id="chatmember">ChatMember</h4>
<p>Класс участника чата. Содержит базовую информацию об участнике.</p>
<h6 id="-">Описание методов класса</h6>
<table>
<thead>
<tr>
<th>Метод</th>
<th>Доступ</th>
<th>Возвращаемое значение</th>
<th>Аргументы</th>
<th>Описание</th>
</tr>
</thead>
<tbody>
<tr>
<td>AuthMember</td>
<td>public</td>
<td>void</td>
<td>const std::string &amp;</td>
<td>Производит авторизацию пользователя, в качестве аргумента принимает его ник</td>
</tr>
<tr>
<td>GetNick</td>
<td>public</td>
<td>const std::string&amp;</td>
<td>-</td>
<td>Геттер для приватного поля с ником участника</td>
</tr>
<tr>
<td>IsAuthorized</td>
<td>public</td>
<td>bool</td>
<td>-</td>
<td>Возвращает true, если участник авторизован, иначе false</td>
</tr>
</tbody>
</table>
<p></a>
</a>
<a name="subparagraph2"></p>
<h2 id="-">Модульные тесты</h2>
<p>Для модульного тестирования в проекте используются библиотеки gtest и gmock
<a name="subpar21"></p>
<h3 id="logger">Logger</h3>
<p>Содержит тесты для класса Logger (файл <code>logger_test.cpp</code>)
Используется класс-заглушка <code>MockLogger</code> и Test-Fixture класс <code>LoggerTest</code><br></a>
<a name="subpar22"></p>
<h3 id="chatmember">ChatMember</h3>
<p>Содержит тесты для класса ChatMember (файл <code>chat_member_test.cpp</code>)
Используется Test-Fixture класс <code>ChatMemberTest</code><br></a>
<a name="subpar23"></p>
<h3 id="socket">Socket</h3>
<p>Содержит тесты для класса Socket (файл <code>socket_test.cpp</code>)
Используется класс-заглушка <code>MockSocket</code><br></a>
</a>
<a name="build"></p>
<h2 id="-">Сборка проекта</h2>
<p>В проекте используются библиотеки boost (asio,log), а также google tests. Для сборки проекта используется система сборки cmake. </p>
<blockquote>
<p>Для удобства сборки написан скрипт BUILD_PROJECT.sh
Данный скрипт автоматически установит необходимые библиотеки, если у пользователя их не будет и соберет проект, затем очистит его от файлов Cmake и make.</p>
</blockquote>
<h3 id="-">Ниже представлен процесс сборки проекта:</h3>
<p>Клонируем репозиторий:</p>

    git clone https://github.com/danognev/TCPChat

<p>Переходим в директорию со скачанным репозиторием:</p>

    cd TCPChat
<p>Запускаем скрипт для сборки проекта:</p>

    bash BUILD_PROJECT.sh

<p>Ждем завершения сборки. Собранный проект располагается в директории: <code>build</code>. Исполняемые файлы сервера и модульных тестов расположены в каталоге <code>build/bin</code></p>
<h2 id="-">Запуск исполняемых файлов</h2>
<p>Для запуска сервера или модульных тестов необходимо перейти в директорию с исполняемыми файлами:</p>

    cd build/bin

<p>Затем для запуска сервера необходимо ввести:</p>

    ./Server [порт]

<p>Для запуска модульных тестов: </p>

    ./UnitTests

<h2 id="-">Подключение к серверу</h2>
<p>После запуска сервера, например, <code>./Server 3333</code>, мы можем подключиться к нему используя, как вариант, утилиту telnet. Для этого необходимо прописать:</p>

    telnet [ip адрес сервера] [порт]

<p>Вы успешно подключились к чату!</p>
</a>
</a>
