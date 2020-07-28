// Extension lib defines
#define EXTENSION_NAME CutoutsExt
#define LIB_NAME "CutoutsExt"
#define MODULE_NAME "cutouts"

// Defold SDK
#define DLIB_LOG_DOMAIN LIB_NAME
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_ANDROID)

static JNIEnv* Attach()
{
    JNIEnv* env;
    dmGraphics::GetNativeAndroidJavaVM()->AttachCurrentThread(&env, NULL);
    return env;
}

static void Detach()
{
    dmGraphics::GetNativeAndroidJavaVM()->DetachCurrentThread();
}

static jclass GetClass(JNIEnv* env, const char* classname)
{
    jclass activity_class = env->FindClass("android/app/NativeActivity");
    jmethodID get_class_loader = env->GetMethodID(activity_class,"getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject cls = env->CallObjectMethod(dmGraphics::GetNativeAndroidActivity(), get_class_loader);
    jclass class_loader = env->FindClass("java/lang/ClassLoader");
    jmethodID find_class = env->GetMethodID(class_loader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    jstring str_class_name = env->NewStringUTF(classname);
    jclass outcls = (jclass)env->CallObjectMethod(cls, find_class, str_class_name);
    env->DeleteLocalRef(str_class_name);
    return outcls;
}

static int GetOffsets(lua_State* L)
{
    JNIEnv* env = Attach();
        
    jclass cls = GetClass(env, "com.defold.android.Cutouts");
    jmethodID get_offsets = env->GetStaticMethodID(cls, "GetOffsets", "(Landroid/app/Activity;)[I");

    jintArray jOffsets = (jintArray)env->CallStaticObjectMethod(cls, get_offsets, dmGraphics::GetNativeAndroidActivity());
    jint *elements = env->GetIntArrayElements(jOffsets, NULL);

    lua_pushnumber(L, elements[0]);
    lua_pushnumber(L, elements[1]);
    lua_pushnumber(L, elements[2]);
    lua_pushnumber(L, elements[3]);

    env->ReleaseIntArrayElements(jOffsets, elements, JNI_ABORT);
    Detach();
    
    return 4;
}

// Functions exposed to Lua
static const luaL_reg CutoutExt_methods[] =
{
    {"get_safe_offsets", GetOffsets},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, MODULE_NAME, CutoutExt_methods);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

static dmExtension::Result AppInitializeExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeExtension(dmExtension::Params* params)
{
    // Init Lua
    LuaInit(params->m_L);
    printf("Registered %s Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeExtension(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

#else

static dmExtension::Result AppInitializeExtension(dmExtension::AppParams* params)
{
    dmLogWarning("Registered %s (null) Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeExtension(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeExtension(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

#endif

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, AppInitializeExtension, AppFinalizeExtension, InitializeExtension, 0, 0, FinalizeExtension)
