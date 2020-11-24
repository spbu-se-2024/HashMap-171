//
// Created by trefi on 20.11.2020.
//

#ifndef SHA_ALGORITHM_SHA_H
#define SHA_ALGORITHM_SHA_H

#include "stddef.h"

void SHA_1(char* message, unsigned long long size, unsigned int* out_hash); //размер должен быть в байтах (сообщения с нецелым количеством байт не поддерживаются)


#endif //SHA_ALGORITHM_SHA_H
