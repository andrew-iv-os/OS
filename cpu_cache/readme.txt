#define GET_ADDR( x, y) ( (y & PAGE_MASK) << PAGES_PER_LINE_LOG2 << PAGE_SIZE_LOG4 )/* здесь мы вычисляем смещение в памяти для нашей страницы которое задаётся y координатой*/ 

+ ( ( x & PAGE_MASK ) << PAGE_SIZE_LOG4 ) /*тоже самое для x*/
+ ( ( y & PAGE_OFFSET_MASK ) << PAGE_SIZE_LOG4 ) + ( x & PAGE_OFFSET_MASK )/*здесь считается смещения внутри страницы*/

Добавил 2 теста подтверждающие что моя формула работает верно.
