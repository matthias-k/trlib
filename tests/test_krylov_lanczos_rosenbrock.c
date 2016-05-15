#include "trlib_test.h"

START_TEST (test_rosenbrock)
{
    struct trlib_test_qp qp;
    trlib_test_malloc_qp(TRLIB_TEST_DENSE_QP, TRLIB_TEST_SOLVER_KRYLOV, 100, 10*100, &qp);
    qp.verbose = 1;
    qp.tol_rel_i = TRLIB_EPS;
    qp.tol_rel_b = TRLIB_EPS;

    struct trlib_test_problem_dense* problem = (struct trlib_test_problem_dense*) qp.problem;
    trlib_flt_t *hess = problem->hess; trlib_flt_t *grad = problem->grad;

    hess[0] = 8.020000e+02;
    hess[1] = -4.000000e+02;
    hess[100] = -4.000000e+02;
    hess[101] = 1.002000e+03;
    hess[102] = -4.000000e+02;
    hess[201] = -4.000000e+02;
    hess[202] = 1.002000e+03;
    hess[203] = -4.000000e+02;
    hess[302] = -4.000000e+02;
    hess[303] = 1.002000e+03;
    hess[304] = -4.000000e+02;
    hess[403] = -4.000000e+02;
    hess[404] = 1.002000e+03;
    hess[405] = -4.000000e+02;
    hess[504] = -4.000000e+02;
    hess[505] = 1.002000e+03;
    hess[506] = -4.000000e+02;
    hess[605] = -4.000000e+02;
    hess[606] = 1.002000e+03;
    hess[607] = -4.000000e+02;
    hess[706] = -4.000000e+02;
    hess[707] = 1.002000e+03;
    hess[708] = -4.000000e+02;
    hess[807] = -4.000000e+02;
    hess[808] = 1.002000e+03;
    hess[809] = -4.000000e+02;
    hess[908] = -4.000000e+02;
    hess[909] = 1.002000e+03;
    hess[910] = -4.000000e+02;
    hess[1009] = -4.000000e+02;
    hess[1010] = 1.002000e+03;
    hess[1011] = -4.000000e+02;
    hess[1110] = -4.000000e+02;
    hess[1111] = 1.002000e+03;
    hess[1112] = -4.000000e+02;
    hess[1211] = -4.000000e+02;
    hess[1212] = 1.002000e+03;
    hess[1213] = -4.000000e+02;
    hess[1312] = -4.000000e+02;
    hess[1313] = 1.002000e+03;
    hess[1314] = -4.000000e+02;
    hess[1413] = -4.000000e+02;
    hess[1414] = 1.002000e+03;
    hess[1415] = -4.000000e+02;
    hess[1514] = -4.000000e+02;
    hess[1515] = 1.002000e+03;
    hess[1516] = -4.000000e+02;
    hess[1615] = -4.000000e+02;
    hess[1616] = 1.002000e+03;
    hess[1617] = -4.000000e+02;
    hess[1716] = -4.000000e+02;
    hess[1717] = 1.002000e+03;
    hess[1718] = -4.000000e+02;
    hess[1817] = -4.000000e+02;
    hess[1818] = 1.002000e+03;
    hess[1819] = -4.000000e+02;
    hess[1918] = -4.000000e+02;
    hess[1919] = 1.002000e+03;
    hess[1920] = -4.000000e+02;
    hess[2019] = -4.000000e+02;
    hess[2020] = 1.002000e+03;
    hess[2021] = -4.000000e+02;
    hess[2120] = -4.000000e+02;
    hess[2121] = 1.002000e+03;
    hess[2122] = -4.000000e+02;
    hess[2221] = -4.000000e+02;
    hess[2222] = 1.002000e+03;
    hess[2223] = -4.000000e+02;
    hess[2322] = -4.000000e+02;
    hess[2323] = 1.002000e+03;
    hess[2324] = -4.000000e+02;
    hess[2423] = -4.000000e+02;
    hess[2424] = 1.002000e+03;
    hess[2425] = -4.000000e+02;
    hess[2524] = -4.000000e+02;
    hess[2525] = 1.002000e+03;
    hess[2526] = -4.000000e+02;
    hess[2625] = -4.000000e+02;
    hess[2626] = 1.002000e+03;
    hess[2627] = -4.000000e+02;
    hess[2726] = -4.000000e+02;
    hess[2727] = 1.002000e+03;
    hess[2728] = -4.000000e+02;
    hess[2827] = -4.000000e+02;
    hess[2828] = 1.002000e+03;
    hess[2829] = -4.000000e+02;
    hess[2928] = -4.000000e+02;
    hess[2929] = 1.002000e+03;
    hess[2930] = -4.000000e+02;
    hess[3029] = -4.000000e+02;
    hess[3030] = 1.002000e+03;
    hess[3031] = -4.000000e+02;
    hess[3130] = -4.000000e+02;
    hess[3131] = 1.002000e+03;
    hess[3132] = -4.000000e+02;
    hess[3231] = -4.000000e+02;
    hess[3232] = 1.002000e+03;
    hess[3233] = -4.000000e+02;
    hess[3332] = -4.000000e+02;
    hess[3333] = 1.002000e+03;
    hess[3334] = -4.000000e+02;
    hess[3433] = -4.000000e+02;
    hess[3434] = 1.002000e+03;
    hess[3435] = -4.000000e+02;
    hess[3534] = -4.000000e+02;
    hess[3535] = 1.002000e+03;
    hess[3536] = -4.000000e+02;
    hess[3635] = -4.000000e+02;
    hess[3636] = 1.002000e+03;
    hess[3637] = -4.000000e+02;
    hess[3736] = -4.000000e+02;
    hess[3737] = 1.002000e+03;
    hess[3738] = -4.000000e+02;
    hess[3837] = -4.000000e+02;
    hess[3838] = 1.002000e+03;
    hess[3839] = -4.000000e+02;
    hess[3938] = -4.000000e+02;
    hess[3939] = 1.002000e+03;
    hess[3940] = -4.000000e+02;
    hess[4039] = -4.000000e+02;
    hess[4040] = 1.002000e+03;
    hess[4041] = -4.000000e+02;
    hess[4140] = -4.000000e+02;
    hess[4141] = 1.002000e+03;
    hess[4142] = -4.000000e+02;
    hess[4241] = -4.000000e+02;
    hess[4242] = 1.002000e+03;
    hess[4243] = -4.000000e+02;
    hess[4342] = -4.000000e+02;
    hess[4343] = 1.002000e+03;
    hess[4344] = -4.000000e+02;
    hess[4443] = -4.000000e+02;
    hess[4444] = 1.002000e+03;
    hess[4445] = -4.000000e+02;
    hess[4544] = -4.000000e+02;
    hess[4545] = 1.002000e+03;
    hess[4546] = -4.000000e+02;
    hess[4645] = -4.000000e+02;
    hess[4646] = 1.002000e+03;
    hess[4647] = -4.000000e+02;
    hess[4746] = -4.000000e+02;
    hess[4747] = 1.002000e+03;
    hess[4748] = -4.000000e+02;
    hess[4847] = -4.000000e+02;
    hess[4848] = 1.002000e+03;
    hess[4849] = -4.000000e+02;
    hess[4948] = -4.000000e+02;
    hess[4949] = 1.002000e+03;
    hess[4950] = -4.000000e+02;
    hess[5049] = -4.000000e+02;
    hess[5050] = 1.002000e+03;
    hess[5051] = -4.000000e+02;
    hess[5150] = -4.000000e+02;
    hess[5151] = 1.002000e+03;
    hess[5152] = -4.000000e+02;
    hess[5251] = -4.000000e+02;
    hess[5252] = 1.002000e+03;
    hess[5253] = -4.000000e+02;
    hess[5352] = -4.000000e+02;
    hess[5353] = 1.002000e+03;
    hess[5354] = -4.000000e+02;
    hess[5453] = -4.000000e+02;
    hess[5454] = 1.002000e+03;
    hess[5455] = -4.000000e+02;
    hess[5554] = -4.000000e+02;
    hess[5555] = 1.002000e+03;
    hess[5556] = -4.000000e+02;
    hess[5655] = -4.000000e+02;
    hess[5656] = 1.002000e+03;
    hess[5657] = -4.000000e+02;
    hess[5756] = -4.000000e+02;
    hess[5757] = 1.002000e+03;
    hess[5758] = -4.000000e+02;
    hess[5857] = -4.000000e+02;
    hess[5858] = 1.002000e+03;
    hess[5859] = -4.000000e+02;
    hess[5958] = -4.000000e+02;
    hess[5959] = 1.002000e+03;
    hess[5960] = -4.000000e+02;
    hess[6059] = -4.000000e+02;
    hess[6060] = 1.002000e+03;
    hess[6061] = -4.000000e+02;
    hess[6160] = -4.000000e+02;
    hess[6161] = 1.002000e+03;
    hess[6162] = -4.000000e+02;
    hess[6261] = -4.000000e+02;
    hess[6262] = 1.002000e+03;
    hess[6263] = -4.000000e+02;
    hess[6362] = -4.000000e+02;
    hess[6363] = 1.002000e+03;
    hess[6364] = -4.000000e+02;
    hess[6463] = -4.000000e+02;
    hess[6464] = 1.002000e+03;
    hess[6465] = -4.000000e+02;
    hess[6564] = -4.000000e+02;
    hess[6565] = 1.002000e+03;
    hess[6566] = -4.000000e+02;
    hess[6665] = -4.000000e+02;
    hess[6666] = 1.002000e+03;
    hess[6667] = -4.000000e+02;
    hess[6766] = -4.000000e+02;
    hess[6767] = 1.002000e+03;
    hess[6768] = -4.000000e+02;
    hess[6867] = -4.000000e+02;
    hess[6868] = 1.002000e+03;
    hess[6869] = -4.000000e+02;
    hess[6968] = -4.000000e+02;
    hess[6969] = 1.002000e+03;
    hess[6970] = -4.000000e+02;
    hess[7069] = -4.000000e+02;
    hess[7070] = 1.002000e+03;
    hess[7071] = -4.000000e+02;
    hess[7170] = -4.000000e+02;
    hess[7171] = 1.002000e+03;
    hess[7172] = -4.000000e+02;
    hess[7271] = -4.000000e+02;
    hess[7272] = 1.002000e+03;
    hess[7273] = -4.000000e+02;
    hess[7372] = -4.000000e+02;
    hess[7373] = 1.002000e+03;
    hess[7374] = -4.000000e+02;
    hess[7473] = -4.000000e+02;
    hess[7474] = 1.002000e+03;
    hess[7475] = -4.000000e+02;
    hess[7574] = -4.000000e+02;
    hess[7575] = 1.002000e+03;
    hess[7576] = -4.000000e+02;
    hess[7675] = -4.000000e+02;
    hess[7676] = 1.002000e+03;
    hess[7677] = -3.999999e+02;
    hess[7776] = -3.999999e+02;
    hess[7777] = 1.002000e+03;
    hess[7778] = -3.999999e+02;
    hess[7877] = -3.999999e+02;
    hess[7878] = 1.001999e+03;
    hess[7879] = -3.999998e+02;
    hess[7978] = -3.999998e+02;
    hess[7979] = 1.001999e+03;
    hess[7980] = -3.999997e+02;
    hess[8079] = -3.999997e+02;
    hess[8080] = 1.001997e+03;
    hess[8081] = -3.999993e+02;
    hess[8180] = -3.999993e+02;
    hess[8181] = 1.001995e+03;
    hess[8182] = -3.999987e+02;
    hess[8281] = -3.999987e+02;
    hess[8282] = 1.001989e+03;
    hess[8283] = -3.999974e+02;
    hess[8382] = -3.999974e+02;
    hess[8383] = 1.001979e+03;
    hess[8384] = -3.999947e+02;
    hess[8483] = -3.999947e+02;
    hess[8484] = 1.001957e+03;
    hess[8485] = -3.999893e+02;
    hess[8584] = -3.999893e+02;
    hess[8585] = 1.001915e+03;
    hess[8586] = -3.999786e+02;
    hess[8685] = -3.999786e+02;
    hess[8686] = 1.001829e+03;
    hess[8687] = -3.999571e+02;
    hess[8786] = -3.999571e+02;
    hess[8787] = 1.001656e+03;
    hess[8788] = -3.999138e+02;
    hess[8887] = -3.999138e+02;
    hess[8888] = 1.001310e+03;
    hess[8889] = -3.998271e+02;
    hess[8988] = -3.998271e+02;
    hess[8989] = 1.000615e+03;
    hess[8990] = -3.996531e+02;
    hess[9089] = -3.996531e+02;
    hess[9090] = 9.992239e+02;
    hess[9091] = -3.993042e+02;
    hess[9190] = -3.993042e+02;
    hess[9191] = 9.964390e+02;
    hess[9192] = -3.986050e+02;
    hess[9291] = -3.986050e+02;
    hess[9292] = 9.908799e+02;
    hess[9293] = -3.972055e+02;
    hess[9392] = -3.972055e+02;
    hess[9393] = 9.798408e+02;
    hess[9394] = -3.944115e+02;
    hess[9493] = -3.944115e+02;
    hess[9494] = 9.581498e+02;
    hess[9495] = -3.888626e+02;
    hess[9594] = -3.888626e+02;
    hess[9595] = 9.164187e+02;
    hess[9596] = -3.779559e+02;
    hess[9695] = -3.779559e+02;
    hess[9696] = 8.394449e+02;
    hess[9697] = -3.569612e+02;
    hess[9796] = -3.569612e+02;
    hess[9797] = 7.088901e+02;
    hess[9798] = -3.182149e+02;
    hess[9897] = -3.182149e+02;
    hess[9898] = 5.220029e+02;
    hess[9899] = -2.525654e+02;
    hess[9998] = -2.525654e+02;
    hess[9999] = 2.000000e+02;
    
    grad[0] = -0.000000e+00;
    grad[1] = 0.000000e+00;
    grad[2] = 0.000000e+00;
    grad[3] = 0.000000e+00;
    grad[4] = 0.000000e+00;
    grad[5] = 0.000000e+00;
    grad[6] = 0.000000e+00;
    grad[7] = 0.000000e+00;
    grad[8] = 0.000000e+00;
    grad[9] = 0.000000e+00;
    grad[10] = 0.000000e+00;
    grad[11] = 0.000000e+00;
    grad[12] = 0.000000e+00;
    grad[13] = 0.000000e+00;
    grad[14] = 0.000000e+00;
    grad[15] = 0.000000e+00;
    grad[16] = 0.000000e+00;
    grad[17] = 0.000000e+00;
    grad[18] = 4.440892e-14;
    grad[19] = -1.112443e-13;
    grad[20] = 8.881784e-14;
    grad[21] = -1.112443e-13;
    grad[22] = 8.881784e-14;
    grad[23] = -2.000622e-13;
    grad[24] = 4.001244e-13;
    grad[25] = -6.001866e-13;
    grad[26] = 5.782042e-13;
    grad[27] = 1.332268e-13;
    grad[28] = -1.755707e-12;
    grad[29] = 4.801048e-12;
    grad[30] = -9.558132e-12;
    grad[31] = 1.591571e-11;
    grad[32] = -2.358491e-11;
    grad[33] = 3.218759e-11;
    grad[34] = -4.150169e-11;
    grad[35] = 5.143796e-11;
    grad[36] = -6.210810e-11;
    grad[37] = 7.340040e-11;
    grad[38] = -8.531531e-11;
    grad[39] = 9.758594e-11;
    grad[40] = -1.097453e-10;
    grad[41] = 1.217266e-10;
    grad[42] = -1.335752e-10;
    grad[43] = 1.434239e-10;
    grad[44] = -1.583282e-10;
    grad[45] = 2.162204e-10;
    grad[46] = -3.490745e-10;
    grad[47] = 5.136895e-10;
    grad[48] = -6.529071e-10;
    grad[49] = 7.924226e-10;
    grad[50] = -9.258831e-10;
    grad[51] = 1.019474e-09;
    grad[52] = -1.087278e-09;
    grad[53] = 1.153054e-09;
    grad[54] = -1.196119e-09;
    grad[55] = 1.202595e-09;
    grad[56] = -1.209676e-09;
    grad[57] = 1.216274e-09;
    grad[58] = -1.209219e-09;
    grad[59] = 1.201522e-09;
    grad[60] = -1.209318e-09;
    grad[61] = 1.214130e-09;
    grad[62] = -1.172520e-09;
    grad[63] = 1.274698e-09;
    grad[64] = -8.110472e-10;
    grad[65] = 2.612295e-09;
    grad[66] = 4.666195e-09;
    grad[67] = 2.406015e-08;
    grad[68] = 8.725516e-08;
    grad[69] = 3.272812e-07;
    grad[70] = 1.093156e-06;
    grad[71] = 2.964843e-06;
    grad[72] = 4.198116e-06;
    grad[73] = -1.272495e-05;
    grad[74] = -5.589255e-05;
    grad[75] = 9.264819e-06;
    grad[76] = -1.170945e-05;
    grad[77] = -4.543612e-07;
    grad[78] = 3.178579e-06;
    grad[79] = 1.102944e-06;
    grad[80] = -3.741205e-06;
    grad[81] = -3.213094e-06;
    grad[82] = 2.050033e-06;
    grad[83] = 4.037614e-06;
    grad[84] = -2.597651e-07;
    grad[85] = -4.177398e-06;
    grad[86] = -1.622876e-06;
    grad[87] = 3.565794e-06;
    grad[88] = 3.795689e-06;
    grad[89] = 3.726634e-07;
    grad[90] = 5.284732e-06;
    grad[91] = 3.778260e-05;
    grad[92] = 1.543671e-04;
    grad[93] = 5.898034e-04;
    grad[94] = 2.165138e-03;
    grad[95] = 6.438287e-03;
    grad[96] = 4.034755e-03;
    grad[97] = -1.115721e-01;
    grad[98] = -3.632669e-01;
    grad[99] = -5.282784e-01;


    qp.radius = 0.2;

    trlib_test_solve_check_qp(&qp, "Lanczos Rosenbrock Coldstart", 1e7*TRLIB_EPS, 1e16*TRLIB_EPS);

    qp.radius = 0.1;
    qp.reentry = 1;
    trlib_test_solve_check_qp(&qp, "Lanczos Rosenbrock Warmstart", 1e7*TRLIB_EPS, 1e16*TRLIB_EPS);
    trlib_test_free_qp(&qp);
}
END_TEST

Suite *krylov_lanczos_rosenbrock_suite(void)
{
    Suite *s;
    TCase *tc_core;
    s = suite_create("Krylov Suite Lanczos Rosenbrock");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_rosenbrock);
    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;
    s = krylov_lanczos_rosenbrock_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed==0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
