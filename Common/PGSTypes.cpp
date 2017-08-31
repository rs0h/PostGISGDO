// Copyright 2011 Intergraph Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "PGSTypes.hpp"
#include <math.h>

int GetOrient(double *v1, double *v2)
{
    double alpha[3];
    for(int i = 0; i < 3; i++)
    {
        if(fabs(v2[i]) > gdPrecLim) alpha[i] = v1[i]/v2[i];
        else alpha[i] = 0.0;
    }
    int k = 0;
    if(fabs(alpha[1]) > fabs(alpha[0])) k = 1;
    if(fabs(alpha[2]) > fabs(alpha[k])) k = 2;
    if(alpha[k] < -gdPrecLim) return(-1);
    if(alpha[k] < gdPrecLim) return(0);
    return(1);
}
