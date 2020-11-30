# Как пользоваться CuTest?

1. Пишите Ваши тесты в **test-<your_part>.c** (файлы уже созданы).

2. Пользуйтесь **разделителями секций** (примеры есть в файлах).

3. Сигнатура Ваших тестов должна быть такой:

   ```c
   // В ней будут вызываться Ваши Assert макросы
   void TestApp_Dummy_Eq(CuTest *tc);
   ```

4. Доступные **макросы** (*приведены в виде функций для подсветки и понимания типов, **НА ДЕЛЕ ЯВЛЯЮТСЯ МАКРОСАМИ***):

   ```c
   void CuFail(CuTest *tc, const char *message);
   
   void CuAssert(CuTest *tc, const char *message, int condition);
   void CuAssertTrue(CuTest *tc, int condition);
   
   void CuAssertStrEquals(CuTest *tc, char *expected, char *actual);
   void CuAssertStrEquals_Msg(CuTest *tc, const char *message, char *expected, char *actual);
   
   void CuAssertIntEquals(CuTest *tc, int expected, int actual);
   void CuAssertIntEquals_Msg(CuTest *tc, const char *message, int expected, int actual);
   
   void CuAssertDblEquals(CuTest *tc, double expected, double actual, double delta);
   void CuAssertDblEquals_Msg(CuTest *tc, const char *message, double expected, double actual, double delta);
   
   void CuAssertPtrEquals(CuTest *tc, void *expected, void *actual);
   void CuAssertPtrEquals_Msg(CuTest *tc, const char *message, void *expected, void *actual);
   
   void CuAssertPtrNotNull(CuTest *tc, void *pointer);
   void CuAssertPtrNotNullMsg(CuTest *tc, const char *message, void *pointer);
   ```

5. Все Ваши тесты нужно внедрить в Ваш **Suite** (уже есть в файле):

   ```c
   CuSuite *AppGetSuite() {
       CuSuite *suite = CuSuiteNew();
   
       // SUITE_ADD_TEST(suite, <NAME OF YOUR TEST #1>);
       // SUITE_ADD_TEST(suite, <NAME OF YOUR TEST #2>);
       // SUITE_ADD_TEST(suite, <NAME OF YOUR TEST #3>);
       // SUITE_ADD_TEST(suite, <NAME OF YOUR TEST #4>);
       // SUITE_ADD_TEST(suite, <NAME OF YOUR TEST #5>);
   
       return suite;
   }
   ```

6. **Старайтесь** следовать именованию тестов: `Test<YourPartName>_<SectionName>_<TestName>_<Result>`.

   Это конечно не обязательно, но на практике сделает Вашу жизнь намного легче :wink:.

7. Всё что я мог пропустить Вы можете найти либо [здесь](https://github.com/spbu-se-2024/HashMap-171/blob/master/cutest-1.5), либо написав мне (@AzimMuradov / @azim_muradov).
