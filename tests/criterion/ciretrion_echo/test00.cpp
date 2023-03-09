#include <criterion/criterion.h>
#include <echo.h>

Test(misc, echo) {
    echo("Test");
}

Test(misc, echo2) {
    echo2();
}

Test(misc, passing) {
    cr_assert(1);
}
