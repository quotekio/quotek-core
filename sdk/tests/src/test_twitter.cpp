/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/datasources/twitter.hpp>
#include <iostream>

void test_twitter() {

  quotek::datasource::twitter tw("hp9gZjRQzuz1vq88P0vqevEE2",
                                 "0R9VnDuJn0yCnfdXXUHaG3fJcFx5cf5ARpES49ThejYmKb62KS",
                                 "2998838957-7yfKiu7bUIasBQXasI4IXVOiaVoLyHgy2WIRiIo",
                                 "szrpOO7X8f66irda1HyjLlpkDGyAW2R9TfSy3ZFhJJFOb");

}

void test_fetch() {

  quotek::datasource::twitter tw("hp9gZjRQzuz1vq88P0vqevEE2",
                                 "0R9VnDuJn0yCnfdXXUHaG3fJcFx5cf5ARpES49ThejYmKb62KS",
                                 "2998838957-7yfKiu7bUIasBQXasI4IXVOiaVoLyHgy2WIRiIo",
                                 "szrpOO7X8f66irda1HyjLlpkDGyAW2R9TfSy3ZFhJJFOb");

  std::vector<quotek::data::news> lnews = tw.fetch("wintermew", 5);

  assert(lnews.size() == 5) ;
  assert(lnews[0].content != "");

  std::cout << lnews[1].content << std::endl;

}



int main() {
  test_twitter();
  test_fetch();
}
