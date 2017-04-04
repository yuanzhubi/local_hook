#ifndef LOCAL_HOOK_H_
#define LOCAL_HOOK_H_

#define MACRO_TOSTRING_FOR_HOOK(x) MACRO_TOSTRING_FOR_HOOK1(x)
#define MACRO_TOSTRING_FOR_HOOK1(x) #x
#define MACRO_JOIN_FOR_HOOK3(x,y,z) x##y##z
#define MACRO_JOIN_FOR_HOOK4(x,y,z,w) x##y##z##w

//MACRO API:
//LOCAL_HOOK(src_function_name, dest_function_name) : use it out of function
//LOCAL_HOOK_CPP(src_function_name, dest_function_name) : use it out of function
//LOCAL_HOOK_INIT(src_function_name) : use it out of function
//LOCAL_HOOK_START(src_function_name, dest_function_ptr) : use it in a function and the second argument is a function pointer!
//LOCAL_HOOK_FUNCTIONPOINTER_SYNC(src_function_name) : use it out of function

//local_hook is to help you hook a exported "C" function that was compiled into a dynamic library at x86 or x64 platform in elf format  "locally" !
//Your process space can be separated into following space models: an executable file, several dynamic libraries.
//Here "locally" means you can  hook  the exported function only in one unit.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//局部“挂钩”可以让你“局部”地对一个elf动态库导出的C函数进行“挂钩”。
//我们可以把进程地址空间划分为以下地址模块：一个可执行文件，多个动态库。
//这里我们所说的“局部”即是让“挂钩行为”仅在一个地址模块中生效。

//#include <stdio.h>
//LOCAL_HOOK(puts, myputs) 

//That's all！
//这就完了！


//You have to use LOCAL_HOOK out of function filed. The hook will happen before the main function, or after dlopen finish if the codes is compiled into a dynamic library.
//If your version of GCC does not support __attribute__ ((constructor)), you can use LOCAL_HOOK_CPP in a c++ source file instead.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//你得在函数体外使用LOCAL_HOOK，这样“挂钩”行为会在main函数执行之前生效，在dlopen完成之后（如果你这个代码是被编译进了动态库）生效。
//如果你的GCC版本不支持__attribute__ ((constructor)), 那么你可以在一个C++代码文件里使用LOCAL_HOOK_CPP来代替LOCAL_HOOK。


//But this is not perfect. If other models use function pointer of puts, our hook may affects them.
//The reason is due to the dynamic library linker wants the function pointer of the one function in every models are same, implemented by GOT copy. 
//But seldom did programmers use function pointer to call a function in GLIBC? Most of our hook goals are there.
//But we provide LOCAL_HOOK_FUNCTIONPOINTER_SYNC(puts) to help you remove the side affect if you face it(Attention the function pointers now are different!) 
//and you need to write it in one of the source file of that model (You can use a new file!), then compile and link it.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//但是这并不完美。如果其它模块使用puts的函数指针,我们的“挂钩”行为有可能会影响到他们的取值,
//原因是动态链接器希望程序在不同模块对于同一个函数获得相同的函数指针值, 后加载的模块总是从先加载的模块中拷贝部分GOT。
//不过话说回来，很少有人通过函数指针去调用GLIBC里的函数吧？我们绝大多数挂钩行为的目标都是在GLIBC那里。
//如果你真的足够不幸遇上这样的动态库，那么我们提供LOCAL_HOOK_FUNCTIONPOINTER_SYNC(puts) 去移除对这个模块的副作用（这时候在不同模块对于这个函数puts获得的函数指针值可真就不同了，
//不过多数情况不太可能有程序使用这个特性吧）。不过需要你把这个句话写到那个模块的某个源码文件（或者新加一个源码文件来包含这句话）中并重新编译和链接这个模块。
//你如果不能重新链接这个闭源库, 那就没办法了。


//For more flexible, you can use LOCAL_HOOK_INIT out of function and LOCAL_HOOK_START in any proper function. Then the hook will happen after LOCAL_HOOK_START executed.
//Either the first hook happened in LOCAL_HOOK or LOCAL_HOOK_CPP or LOCAL_HOOK_START, you can call LOCAL_HOOK_START anywhere to hook second times or more, with any other function.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//如果需要更灵活的使用，你可以在函数体外使用LOCAL_HOOK_INIT而在某个合适的函数中使用LOCAL_HOOK_START。“挂钩”行为会在LOCAL_HOOK_START执行后生效。
//不管第一次的“挂钩”行为发生在LOCAL_HOOK 或 LOCAL_HOOK_CPP 或者 LOCAL_HOOK_START之中，你可以在任何函数中第二次调用或者更多次的调用LOCAL_HOOK_START来重新“挂钩”。


//Attention:
//You can use it in C/C++ language.
//Local hook can be only used to hook a function that was compiled into a dynamic library at x86 or x64 platform in elf format.
//Large code models(SYSTEM V AMD64 ABI) is not supported. (Your GOT is greater than 2GB? The total linked ".so" files should be greater than 50GB in size!)
//How to remove the hook? You need to use dlsym to get the original function address first(after hook you do not have other method to get the original function address),
//and use LOCAL_HOOK_START with the address as 2rd argument.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//注意：
//你可以在C/C++中使用。
//局部挂钩只能对一个在x86或者x64的平台下编译进elf格式的动态库并导出的函数进行挂钩。
//“大型代码模式”(见 SYSTEM V AMD64 ABI) 是不支持的。（简单说吧，难道你链接的动态库超过50G了？）
//需要移除“挂钩”？你需要先通过dlsym获取函数的原始地址("挂钩"之后你没有别的方法获取他的原始地址了)，再把他作为第二个参数调用LOCAL_HOOK_START。


#define LOCAL_HOOK(src_function_name, dest_function_name) \
LOCAL_HOOK_INIT(src_function_name) \
__attribute__ ((constructor)) static void MACRO_JOIN_FOR_HOOK4(hook_, src_function_name, dest_function_name, _init)(){\
   LOCAL_HOOK_START(src_function_name, &(dest_function_name)); \
}

#ifdef __cplusplus
#define LOCAL_HOOK_CPP(src_function_name, dest_function_name) \
LOCAL_HOOK_INIT(src_function_name) \
struct MACRO_JOIN_FOR_HOOK4(hook_, src_function_name, dest_function_name, _struct){ \
   MACRO_JOIN_FOR_HOOK4(hook_, src_function_name, dest_function_name, _struct)(){ \
        LOCAL_HOOK_START(src_function_name, &(dest_function_name)); \
   } \
}; \
static MACRO_JOIN_FOR_HOOK4(hook_, src_function_name, dest_function_name, _struct) MACRO_JOIN_FOR_HOOK4(hook_, src_function_name, dest_function_name, _struct_singleton);
#endif

#define LOCAL_HOOK_GOT_FUNCTION_NAME(src_function_name) MACRO_JOIN_FOR_HOOK3(get_, src_function_name, _got_address)
#define LOCAL_HOOK_PLT_FUNCTION_NAME(src_function_name) MACRO_JOIN_FOR_HOOK3(get_, src_function_name, _plt_address)


//.got.plt section can not be deduced by @GOTPLT relocation so we choose to disassemble the plt instruction.
#ifdef __x86_64__

//Attention we do not support large code models! (GOT is greater than 2GB? The "so" may be greater than 50GB!)
#define GET_GOT_PLT_ADDRESS(plt_address, result) \
    do{ \
        unsigned char* c_plt_address = (unsigned char*)(plt_address); \
        union{ \
            int int_content; \
            char content[4]; \
        }a; \
        result = 0; \
        if((c_plt_address[0] & 0xf8) == 0x48){ \
            ++c_plt_address; \
        } \
        if(c_plt_address[0] == 0xff){ \
           if(c_plt_address[1] == 0x25){ \
                const int opsize = 2; \
                const int instruction_size = opsize + 4; \
                a.content[0] = c_plt_address[opsize + 0]; \
                a.content[1] = c_plt_address[opsize + 1]; \
                a.content[2] = c_plt_address[opsize + 2]; \
                a.content[3] = c_plt_address[opsize + 3]; \
                result = (typeof(result))(c_plt_address + instruction_size + a.int_content); \
            } \
            else if(c_plt_address[1] == 0x24 && c_plt_address[2] == 0x25){ \
                const int opsize = 3; \
                a.content[0] = c_plt_address[opsize + 0]; \
                a.content[1] = c_plt_address[opsize + 1]; \
                a.content[2] = c_plt_address[opsize + 2]; \
                a.content[3] = c_plt_address[opsize + 3]; \
                result = (typeof(result))((long long)a.int_content); \
            } \
        } \
    }while(0)

#endif // __x86_64__

#ifdef __i386__

#define GET_GOT_PLT_ADDRESS(plt_address, result) \
    do{ \
        unsigned char* c_plt_address = (unsigned char*)(plt_address); \
        union{ \
            int int_content; \
            char content[4]; \
        }a; \
        result = 0; \
        if(c_plt_address[0] == 0xff && c_plt_address[1] == 0x25){ \
            const int opsize = 2; \
            a.content[0] = c_plt_address[opsize + 0]; \
            a.content[1] = c_plt_address[opsize + 1]; \
            a.content[2] = c_plt_address[opsize + 2]; \
            a.content[3] = c_plt_address[opsize + 3]; \
            result = (typeof(result))(a.int_content); \
        } \
    }while(0)

#endif // __i386__

#if defined(__pic__ ) || defined(__PIC__) || defined(__pie__) || defined(__PIE__)

#ifdef __x86_64__

#define LOCAL_HOOK_GOT(src_function_name) \
__attribute__ ((noinline, optimize("O2"))) static void** LOCAL_HOOK_GOT_FUNCTION_NAME(src_function_name)(){ \
    register void **result asm ("rax"); \
    __asm__  (      "leaq      "MACRO_TOSTRING_FOR_HOOK(src_function_name)"@GOTPCREL(%rip), %rax;"); \
    return result; \
}

#define LOCAL_HOOK_PLT(src_function_name) \
__attribute__ ((noinline, optimize("O2"))) static void*  LOCAL_HOOK_PLT_FUNCTION_NAME(src_function_name)(){ \
    register void *result asm ("rax"); \
    __asm__  (      "leaq       "MACRO_TOSTRING_FOR_HOOK(src_function_name)"@PLT(%rip), %rax;"); \
    return result; \
}

#endif // __x86_64__


#ifdef __i386__

//Intel c++ compiler result says call-pop that leads invalidation of stack buffer will be better than call-lea that leads real jump.
#define LOCAL_HOOK_GOT(src_function_name) \
__attribute__ ((noinline, optimize("O2"))) static void** LOCAL_HOOK_GOT_FUNCTION_NAME(src_function_name)(){ \
    register void **result asm ("eax"); \
    __asm__  (      "call       1f;" \
                    "1:" \
                    "popl       %eax;" \
                    "2:" \
                    "leal       _GLOBAL_OFFSET_TABLE_+[2b-1b](%eax), %eax;" \
                    "leal       "MACRO_TOSTRING_FOR_HOOK(src_function_name)"@GOT(%eax), %eax;" \
            ); \
    return result; \
}

#define LOCAL_HOOK_PLT(src_function_name) \
__attribute__ ((noinline, optimize("O2"))) static void*  LOCAL_HOOK_PLT_FUNCTION_NAME(src_function_name)(){ \
    register void *result asm ("eax"); \
    __asm__  (      "call       1f;" \
                    "1:" \
                    "popl       %eax;" \
                    "leal       "MACRO_TOSTRING_FOR_HOOK(src_function_name)"@PLT(%eax), %eax;" \
                    "2:" \
                    "leal       2b-1b(%eax), %eax;" \
            ); \
    return result; \
}

#endif // __i386__

#define LOCAL_HOOK_INIT(src_function_name)\
    LOCAL_HOOK_GOT(src_function_name) \
    LOCAL_HOOK_PLT(src_function_name)

#define LOCAL_HOOK_START(src_function_name, dest_function_ptr) \
    do{ \
        void** got_plt_addr; \
        *LOCAL_HOOK_GOT_FUNCTION_NAME(src_function_name)() = (void*)dest_function_ptr; \
        GET_GOT_PLT_ADDRESS(LOCAL_HOOK_PLT_FUNCTION_NAME(src_function_name)(), got_plt_addr); \
        *got_plt_addr = (void*)dest_function_ptr; \
    }while(0)


#define LOCAL_HOOK_FUNCTIONPOINTER_SYNC(src_function_name) \
	LOCAL_HOOK_INIT(src_function_name) \
	__attribute__ ((constructor)) static void MACRO_JOIN_FOR_HOOK3(hook_, src_function_name , _sync)(){\
		 *LOCAL_HOOK_GOT_FUNCTION_NAME(src_function_name)() = LOCAL_HOOK_PLT_FUNCTION_NAME(src_function_name)();\
	}


#else // defined(__pic__ ) || defined(__PIC__) || defined(__pie__) || defined(__PIE__)

//now so easy

#define LOCAL_HOOK_INIT(src_function_name)

#define LOCAL_HOOK_START(src_function_name, dest_function_ptr) \
    do{ \
        void** got_plt_addr; \
        GET_GOT_PLT_ADDRESS(&src_function_name, got_plt_addr); \
        *got_plt_addr = (void*)dest_function_ptr; \
    }while(0)

#endif // defined(__pic__ ) || defined(__PIC__) || defined(__pie__) || defined(__PIE__)

#endif






