#ifndef __LOG2_H__
#define __LOG2_H__

#include "stdint.h"

namespace algorithm::log2{

    unsigned short log2_32(uint32_t x){
        
        if (x < 0x10000){
            if (x < 0x100){
                if (x < 0x10){
                    if (x < 0x4){
                        if (x < 0x2){
                            return 0;
                        }
                        return 1;
                    }
                    if (x < 0x8){
                        return 2;
                    }
                    return 3;
                }
                if (x < 0x40){
                    if (x < 0x20){
                        return 4;
                    }
                    return 5;
                }
                if (x < 0x80){
                    return 6;
                }
                return 7;
            }
            if (x < 0x1000){
                if (x < 0x400){
                    if (x < 0x200){
                        return 8;
                    }
                    return 9;
                }
                if (x < 0x800){
                    return 10;
                }
                return 11;
            }
            if (x < 0x4000){
                if (x < 0x2000){
                    return 12;
                }
                return 13;
            }
            if (x < 0x8000){
                return 14;
            }
            return 15;
        }
        if (x < 0x1000000){
            if (x < 0x100000){
                if (x < 0x40000){
                    if (x < 0x20000){
                        return 16;
                    }
                    return 17;
                }
                if (x < 0x80000){
                    return 18;
                }
                return 19;
            }
            if (x < 0x400000){
                if (x < 0x200000){
                    return 20;
                }
                return 21;
            }
            if (x < 0x800000){
                return 22;
            }
            return 23;
        }
        if (x < 0x10000000){
            if (x < 0x4000000){
                if (x < 0x2000000){
                    return 24;
                }
                return 25;
            }
            if (x < 0x8000000){
                return 26;
            }
            return 27;
        }
        if (x < 0x40000000){
            if (x < 0x20000000){
                return 28;
            }
            return 29;
        }
        if (x < 0x80000000){
            return 30;
        }
        return 31;
    }

    unsigned short log2_16(uint16_t x){
        if (x < 0x100){
            if (x < 0x10){
                if (x < 0x4){
                    if (x < 0x2){
                        return 0;
                    }
                    return 1;
                }
                if (x < 0x8){
                    return 2;
                }
                return 3;
            }
            if (x < 0x40){
                if (x < 0x20){
                    return 4;
                }
                return 5;
            }
            if (x < 0x80){
                return 6;
            }
            return 7;
        }
        if (x < 0x1000){
            if (x < 0x400){
                if (x < 0x200){
                    return 8;
                }
                return 9;
            }
            if (x < 0x800){
                return 10;
            }
            return 11;
        }
        if (x < 0x4000){
            if (x < 0x2000){
                return 12;
            }
            return 13;
        }
        if (x < 0x8000){
            return 14;
        }
        return 15;
    }

    unsigned short log2_8(uint8_t x){
        if (x < 0x10){
            if (x < 0x4){
                if (x < 0x2){
                    return 0;
                }
                return 1;
            }
            if (x < 0x8){
                return 2;
            }
            return 3;
        }
        if (x < 0x40){
            if (x < 0x20){
                return 4;
            }
            return 5;
        }
        if (x < 0x80){
            return 6;
        }
        return 7;
    }

    unsigned short log2_64(uint64_t x){
        if (x < 0x100000000){
            if (x < 0x10000){
                if (x < 0x100){
                    if (x < 0x10){
                        if (x < 0x4){
                            if (x < 0x2){
                                return 0;
                            }
                            return 1;
                        }
                        if (x < 0x8){
                            return 2;
                        }
                        return 3;
                    }
                    if (x < 0x40){
                        if (x < 0x20){
                            return 4;
                        }
                        return 5;
                    }
                    if (x < 0x80){
                        return 6;
                    }
                    return 7;
                }
                if (x < 0x1000){
                    if (x < 0x400){
                        if (x < 0x200){
                            return 8;
                        }
                        return 9;
                    }
                    if (x < 0x800){
                        return 10;
                    }
                    return 11;
                }
                if (x < 0x4000){
                    if (x < 0x2000){
                        return 12;
                    }
                    return 13;
                }
                if (x < 0x8000){
                    return 14;
                }
                return 15;
            }
            if (x < 0x1000000){
                if (x < 0x100000){
                    if (x < 0x40000){
                        if (x < 0x20000){
                            return 16;
                        }
                        return 17;
                    }
                    if (x < 0x80000){
                        return 18;
                    }
                    return 19;
                }
                if (x < 0x400000){
                    if (x < 0x200000){
                        return 20;
                    }
                    return 21;
                }
                if (x < 0x800000){
                    return 22;
                }
                return 23;
            }
            if (x < 0x10000000){
                if (x < 0x4000000){
                    if (x < 0x2000000){
                        return 24;
                    }
                    return 25;
                }
                if (x < 0x8000000){
                    return 26;
                }
                return 27;
            }
            if (x < 0x40000000){
                if (x < 0x20000000){
                    return 28;
                }
                return 29;
            }
            if (x < 0x80000000){
                return 30;
            }
            return 31;
        }
        if (x < 0x1000000000000){
            if (x < 0x10000000000){
                if (x < 0x1000000000){
                    if (x < 0x400000000){
                        if (x < 0x200000000){
                            return 32;
                        }
                        return 33;
                    }
                    if (x < 0x800000000){
                        return 34;
                    }
                    return 35;
                }
                if (x < 0x4000000000){
                    if (x < 0x2000000000){
                        return 36;
                    }
                    return 37;
                }
                if (x < 0x8000000000){
                    return 38;
                }
                return 39;
            }
            if (x < 0x100000000000){
                if (x < 0x40000000000){
                    if (x < 0x20000000000){
                        return 40;
                    }
                    return 41;
                }
                if (x < 0x80000000000){
                    return 42;
                }
                return 43;
            }
            if (x < 0x400000000000){
                if (x < 0x200000000000){
                    return 44;
                }
                return 45;
            }
            if (x < 0x800000000000){
                return 46;
            }
            return 47;
        }
        if (x < 0x100000000000000){
            if (x < 0x10000000000000){
                if (x < 0x4000000000000){
                    if (x < 0x2000000000000){
                        return 48;
                    }
                    return 49;
                }
                if (x < 0x8000000000000){
                    return 50;
                }
                return 51;
            }
            if (x < 0x40000000000000){
                if (x < 0x20000000000000){
                    return 52;
                }
                return 53;
            }
            if (x < 0x80000000000000){
                return 54;
            }
            return 55;
        }
        if (x < 0x1000000000000000){
            if (x < 0x400000000000000){
                if (x < 0x200000000000000){
                    return 56;
                }
                return 57;
            }
            if (x < 0x800000000000000){
                return 58;
            }
            return 59;
        }
        if (x < 0x4000000000000000){
            if (x < 0x2000000000000000){
                return 60;
            }
            return 61;
        }
        if (x < 0x8000000000000000){
            return 62;
        }
        return 63;
    }
}

#endif