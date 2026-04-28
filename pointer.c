#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdlib.h>

static int pointer(lua_State *L) {
    // int* not int, no dereference on NULL check
    int *ptr = (int *)malloc(sizeof(int));
    if (ptr == NULL) {
        return luaL_error(L, "malloc failed");
    }

    printf("enter required number: ");
    scanf("%d", ptr);

    // push the pointer so Lua can hold and return it
    lua_pushlightuserdata(L, ptr);
    return 1;
}
static int freePointer(lua_State *L) {
    if (lua_type(L, 1) != LUA_TLIGHTUSERDATA) {
        return luaL_error(L, "expected pointer, got %s",
            lua_typename(L, lua_type(L, 1)));
    }

    // lua_touserdata reads, cast to int* not int
    int *ptr = (int *)lua_touserdata(L, 1);

    if (ptr == NULL) {
        return luaL_error(L, "pointer is NULL");
    }

    free(ptr);
    ptr = NULL;

    lua_pushboolean(L, 1); // semicolon
    return 1;
}

static const luaL_Reg lib[] = {
    {"pointer", pointer},
    {"freePointer", freePointer},
    {NULL, NULL}
};

__declspec(dllexport) int luaopen_pointerlib(lua_State *L) {
    luaL_newlib(L, lib);
    return 1;
}
