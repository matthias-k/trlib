#include "trlib_tri_factor.h"

int trlib_tri_factor_min(
    int nirblk, int *irblk, double *diag, double *offdiag,
    double *neglin, double radius, 
    int itmax, double tol_rel, int pos_def, int equality,
    int *warm0, double *lam0, int *warm, double *lam,
    int *warm_leftmost, int *ileftmost, double *leftmost,
    int *warm_fac0, double *diag_fac0, double *offdiag_fac0,
    int *warm_fac, double *diag_fac, double *offdiag_fac,
    double *sol0, double *sol, double *ones, double *fwork,
    int refine,
    int verbose, int unicode, char *prefix, FILE *fout,
    long *timing, double *obj, int *iter_newton, int *sub_fail) {
    // use notation of Gould paper
    // h = h(lam) denotes solution of (T+lam I) * h = -lin

    // local variables
    #if TRLIB_MEASURE_TIME
        struct timespec verystart, start, end;
        TRLIB_TIC(verystart)
    #endif
    /* this is based on Theorem 5.8 in Gould paper,
     * the data for the first block has a 0 suffix,
     * the data for the \ell block has a l suffix */
    int n0 = irblk[1];                               // dimension of first block
    int nl;                                          // dimension of block corresponding to leftmost
    int nm0 = irblk[1]-1;                            // length of offdiagonal of first block
    int info_fac = 0;                                // factorization information
    int ret = 0;                                     // return code
    int newton = 0;                                  // perform newton iteration
    double lam_pert = 0.0;                           // perturbation of leftmost eigenvalue as starting value for lam
    double norm_sol0 = 0.0;                          // norm of h_0(lam)
    int interior = 0;                                // solution is interior
    *iter_newton = 0;                                // newton iteration counter
    int jj = 0;                                      // local iteration counter
    double dlam     = 0.0;                           // increment in newton iteration
    int inc = 1;                                     // increment in vector storage
    double *w = fwork;                               // auxiliary vector to be used in newton iteration
    double *diag_lam = fwork+(irblk[nirblk]);        // vector that holds diag + lam, could be saved if we would implement iterative refinement ourselve
    double *work = fwork+2*(irblk[nirblk]);          // workspace for iterative refinement
    double ferr = 0.0;                               // forward  error bound from iterative refinement
    double berr = 0.0;                               // backward error bound from iterative refinement
    double dot = 0.0;                                // save dot products
    double invD_norm_w_sq = 0.0;                     // || w ||_{D^-1}^2

    // FIXME: ensure diverse warmstarts work as expected
    
    // initialization:
    *sub_fail = 0;

    // set sol to 0 as a safeguard
    memset(sol, 0, irblk[nirblk]*sizeof(double));

    // first make sure that lam0, h_0 is accurate
    TRLIB_PRINTLN_1("Solving trust region problem, radius %e; starting on first irreducible block", radius)
    // if only blocks changed that differ from the first then there is nothing to do
    if (nirblk > 1 && *warm0) {
        TRLIB_DNRM2(norm_sol0, &n0, sol0, &inc)
        TRLIB_PRINTLN_1("Solution provided via warmstart, \u03bb\u2080 = %e, \u2016h\u2080\u2016 = %e", *lam0, norm_sol0)
        if (norm_sol0-radius < 0.0) { TRLIB_PRINTLN_1("  violates \u2016h\u2080\u2016 - radius \u2265 0, but is %e, switch to coldstart", norm_sol0-radius) *warm0 = 0; }
        else { newton = 1; }
    }
    if (nirblk == 1 || !*warm0) {
        // seek for lam0, h_0 with (T0+lam0*I) pos def and ||h_0(lam_0)|| = radius

        /* as a first step to initialize the newton iteration,
         *  find such a pair with the losened requierement ||h_0(lam_0)|| >= radius */
        if(*warm0) {
            if(!*warm_fac0) {
                // factorize T + lam0 I
                TRLIB_DCOPY(&n0, diag, &inc, diag_lam, &inc) // diag_lam <-- diag
                TRLIB_DAXPY(&n0, lam0, ones, &inc, diag_lam, &inc) // diag_lam <-- lam0 + diag_lam
                TRLIB_DCOPY(&n0, diag_lam, &inc, diag_fac0, &inc) // diag_fac0 <-- diag_lam
                TRLIB_DCOPY(&nm0, offdiag, &inc, offdiag_fac0, &inc) // offdiag_fac0 <-- offdiag
                TRLIB_DPTTRF(&n0, diag_fac0, offdiag_fac0, &info_fac) // compute factorization
                if (info_fac != 0) { *warm0 = 0; } // factorization failed, switch to coldastart
                else { *warm_fac0 = 1; }
            }
            if(*warm_fac0) {
                // solve for h0(lam0) and compute norm
                TRLIB_DCOPY(&n0, neglin, &inc, sol0, &inc) // sol0 <-- neglin
                TRLIB_DPTTRS(&n0, &inc, diag_fac0, offdiag_fac0, sol0, &n0, &info_fac) // sol <-- (T+lam0 I)^-1 sol
                if(info_fac!=0) { TRLIB_RETURN(TRLIB_TTR_FAIL_LINSOLVE) }
                TRLIB_DNRM2(norm_sol0, &n0, sol0, &inc)
                if (norm_sol0 >= radius) { *warm0 = 1; newton = 1; } else { *warm0 = 0; }
            }
        }
        if(!*warm0) {
            *lam0 = 0.0;
            TRLIB_PRINTLN_1("Coldstart. Seeking suitable initial \u03bb\u2080, starting with 0")
            TRLIB_DCOPY(&n0, diag, &inc, diag_fac0, &inc) // diag_fac0 <-- diag0
            TRLIB_DCOPY(&nm0, offdiag, &inc, offdiag_fac0, &inc) // offdiag_fac0 <-- offdiag0
            TRLIB_DCOPY(&n0, neglin, &inc, sol0, &inc) // sol0 <-- neglin0
            TRLIB_DPTTRF(&n0, diag_fac0, offdiag_fac0, &info_fac) // compute factorization
            if (info_fac == 0) {
                // test if lam0 = 0 is suitable
                TRLIB_DCOPY(&n0, neglin, &inc, sol0, &inc) // sol0 <-- neglin
                TRLIB_DPTTRS(&n0, &inc, diag_fac0, offdiag_fac0, sol0, &n0, &info_fac) // sol0 <-- T0^-1 sol0
                if (info_fac != 0) { TRLIB_RETURN(TRLIB_TTR_FAIL_LINSOLVE) }
                TRLIB_DNRM2(norm_sol0, &n0, sol0, &inc)
                if (norm_sol0<radius && equality) { info_fac = 1; } // in equality case we have to find suitable lam
            }
            if (info_fac != 0) { 
                TRLIB_PRINTLN_1(" \u03bb\u2080 = 0 unsuitable \u2265 get leftmost ev of first block!")
                *sub_fail = trlib_leftmost_irreducible(irblk[1], diag, offdiag, *warm_leftmost, *leftmost, 1000, TRLIB_EPS_POW_75, verbose-2, unicode, " LM ", fout, timing+10, leftmost, &jj); // ferr can safely be overwritten by computed leftmost for the moment as can jj with the number of rp iterations
                // if (*sub_fail != 0) { TRLIB_RETURN(TRLIB_TTR_FAIL_LM) } failure of leftmost: may lead to inefficiency, since what we are doing may be slow...
                // T - leftmost*I is singular, so perturb it a bit to catch factorization; start with small perturbation and increase
                // first enlarge -leftmost to get into the region of positive definiteness
                lam_pert = (1.0+fabs(*leftmost)) * TRLIB_EPS_POW_75;
                while (lam_pert < (1.0+fabs(*leftmost))/TRLIB_EPS) {
                    *lam0 = -(*leftmost) + lam_pert;
                    TRLIB_PRINTLN_2(" attempting factorization with \u03bb\u2080 = %e, pert %e", *lam0, lam_pert)
                    // factorize T + lam I
                    TRLIB_DCOPY(&n0, diag, &inc, diag_lam, &inc) // diag_lam <-- diag
                    TRLIB_DAXPY(&n0, lam0, ones, &inc, diag_lam, &inc) // diag_lam <-- lam + diag_lam
                    TRLIB_DCOPY(&n0, diag_lam, &inc, diag_fac0, &inc) // diag_fac <-- diag_lam
                    TRLIB_DCOPY(&nm0, offdiag, &inc, offdiag_fac0, &inc) // offdiag_fac <-- offdiag
                    TRLIB_DPTTRF(&n0, diag_fac0, offdiag_fac0, &info_fac) // compute factorization
                    if (info_fac == 0) { break; } // factorization possible, exit loop
                    lam_pert = 2.0*lam_pert;
                }
                if (info_fac != 0) { TRLIB_RETURN(TRLIB_TTR_FAIL_FACTOR) } // ensure we finally got a factorization
                // now we have lam0 such that (T0+lam0*I) pos def and is factorized
                // check if ||h0(lam0)|| >= radius (||h0(-leftmost)|| should be infinity)
                TRLIB_DCOPY(&n0, neglin, &inc, sol0, &inc) // sol0 <-- neglin
                TRLIB_DPTTRS(&n0, &inc, diag_fac0, offdiag_fac0, sol0, &n0, &info_fac) // sol0 <-- T0^-1 sol0
                if (info_fac != 0) { TRLIB_RETURN(TRLIB_TTR_FAIL_LINSOLVE) }
                //if (refine) { TRLIB_DPTRFS(&n0, &inc, diag, offdiag, diag_fac0, offdiag_fac0, neglin, &n0, sol0, &n0, &ferr, &berr, work, &info_fac) }
                if (info_fac != 0) { TRLIB_RETURN(TRLIB_TTR_FAIL_LINSOLVE) }
                TRLIB_DNRM2(norm_sol0, &n0, sol0, &inc)
                // if ||h0(lam0)|| < radius, bisect lam0 until ||h0(lam0)|| >= radius
                jj = 0;
                ferr = -(*leftmost) + 0.5*lam_pert; // lower bound on lam0
                berr = *lam0; // uppper bound on lam0
                while ( (info_fac != 0 || norm_sol0 < radius) && jj < 500 && fabs(berr-ferr) > TRLIB_EPS*fabs(*lam0)) {
                    TRLIB_PRINTLN_2(" \u03bb\u2080 = %e unsuitable, \u2016h\u2080\u2016 = %e", *lam0, norm_sol0);
                    *lam0 = .5*(ferr+berr);// + .3*(berr-ferr); jj++;
                    TRLIB_PRINTLN_2(" ")
                    TRLIB_PRINTLN_2(" testing \u03bb\u2080 = %e, enclosure size %e", *lam0, berr - ferr);
                    // factorize T + lam I
                    TRLIB_DCOPY(&n0, diag, &inc, diag_lam, &inc) // diag_lam <-- diag
                    TRLIB_DAXPY(&n0, lam0, ones, &inc, diag_lam, &inc) // diag_lam <-- lam + diag_lam
                    TRLIB_DCOPY(&n0, diag_lam, &inc, diag_fac0, &inc) // diag_fac <-- diag_lam
                    TRLIB_DCOPY(&nm0, offdiag, &inc, offdiag_fac0, &inc) // offdiag_fac <-- offdiag
                    TRLIB_DPTTRF(&n0, diag_fac0, offdiag_fac0, &info_fac) // compute factorization
                    if ( info_fac!= 0) { TRLIB_PRINTLN_2( " factorization failed, increase lower bound, difference %e", *lam0 - ferr) ferr = *lam0; } // factorization failed, increase lower bound
                    else{ 
                        TRLIB_DCOPY(&n0, neglin, &inc, sol0, &inc) // sol0 <-- neglin
                        TRLIB_DPTTRS(&n0, &inc, diag_fac0, offdiag_fac0, sol0, &n0, &info_fac) // sol0 <-- T0^-1 sol0
                        if (info_fac != 0) { TRLIB_RETURN(TRLIB_TTR_FAIL_LINSOLVE) }
                        TRLIB_DNRM2(norm_sol0, &n0, sol0, &inc)
                        if(norm_sol0 < radius) { TRLIB_PRINTLN_2( " norm too small, decrease upper bound, difference %e", berr - *lam0) berr = *lam0; } // still to small, decrease upper bound
                    }
                }
                *warm_fac0 = 0;
            }
            if (norm_sol0<radius) {
                if(*lam0 == 0.0 && !equality) { ret = TRLIB_TTR_CONV_INTERIOR; }
                else { TRLIB_PRINTLN_1(" \u03bb\u2080 = -leftmost = %e with \u2016h\u2080\u2016 = %e turned out to be unsuitable, should be in theory!", *lam0, norm_sol0) TRLIB_RETURN(TRLIB_TTR_FAIL_HARD) }
            }
            else { newton = 1; }
            TRLIB_PRINTLN_1(" \u03bb\u2080 = %e suitable, \u2016h\u2080\u2016 = %e", *lam0, norm_sol0)
        }
    }
    if (newton) {
        /* now a suitable pair lam0, h0 has been found.
         * perform a newton iteration on 0 = 1/||h0(lam0)|| - 1/radius */
        TRLIB_PRINTLN_1("Starting Newton iteration for \u03bb\u2080")
        while (1) {
            /* compute newton correction to lam, by
                (1) Factoring T0 + lam0 I = LDL^T
                (2) Solving (T0+lam0 I)*h0 = -lin
                (3) L*w = h0/||h0||
                (4) compute increment (||h0||-Delta)/Delta/||w||_{D^-1}^2 */
    
            // steps (1) and (2) have already been performed on initializaton or previous iteration
    
            /* step (3) L*w = h/||h||
               compute ||w||_{D^-1}^2 in same loop */
            ferr = 1.0/norm_sol0; TRLIB_DCOPY(&n0, sol0, &inc, w, &inc) TRLIB_DSCAL(&n0, &ferr, w, &inc) // w <-- sol/||sol||
            invD_norm_w_sq = w[0]*w[0]/diag_fac0[0];
            for( jj = 1; jj < n0; ++jj ) { w[jj] = w[jj] - offdiag_fac0[jj-1]*w[jj-1]; invD_norm_w_sq += w[jj]*w[jj]/diag_fac0[jj]; }
    
            // step (4) compute increment (||h||-Delta)/Delta/||w||_{D^-1}^2
            dlam = (norm_sol0-radius)/(radius*invD_norm_w_sq);
    
            // iteration completed
            *iter_newton += 1;
    
            // test if dlam is not tiny or newton limit exceeded, return eventually
            if (fabs(dlam) <= TRLIB_EPS * fabs(*lam0) || *iter_newton > itmax) {
                if (unicode) { TRLIB_PRINTLN_1("%s%e%s%e", "Newton breakdown, d\u03bb = ", dlam, " \u03bb = ", *lam0) }
                else { TRLIB_PRINTLN_1("%s%e%s%e", "Newton breakdown, d\u03bb = ", dlam, " \u03bb = ", *lam0) }
                if(*iter_newton > itmax) { ret = TRLIB_TTR_ITMAX; break; }
                ret = TRLIB_TTR_NEWTON_BREAK; break;
            }
    
            // prepare next iteration
    
            // update lam
            *lam0 += dlam;
    
            // step (1) Factoring T0 + lam0 I = LDL^T
            TRLIB_DCOPY(&n0, diag, &inc, diag_lam, &inc) // diag_lam <-- diag
            TRLIB_DAXPY(&n0, lam0, ones, &inc, diag_lam, &inc) // diag_lam <-- lam + diag_lam
            TRLIB_DCOPY(&n0, diag_lam, &inc, diag_fac0, &inc) // diag_fac <-- diag_lam
            TRLIB_DCOPY(&nm0, offdiag, &inc, offdiag_fac0, &inc) // offdiag_fac <-- offdiag
            TRLIB_DPTTRF(&n0, diag_fac0, offdiag_fac0, &info_fac) // compute factorization
            if (info_fac != 0) { TRLIB_RETURN(TRLIB_TTR_FAIL_FACTOR) }
    
            // step (2) Solving (T+lam I)*h = -lin
            TRLIB_DCOPY(&n0, neglin, &inc, sol0, &inc) // sol <-- neglin
            TRLIB_DPTTRS(&n0, &inc, diag_fac0, offdiag_fac0, sol0, &n0, &info_fac) // sol <-- (T+lam I)^-1 sol
            if (info_fac != 0) { TRLIB_RETURN(TRLIB_TTR_FAIL_LINSOLVE) }
            if (refine) { TRLIB_DPTRFS(&n0, &inc, diag_lam, offdiag, diag_fac0, offdiag_fac0, neglin, &n0, sol0, &n0, &ferr, &berr, work, &info_fac) }
            if (info_fac != 0) { TRLIB_RETURN(TRLIB_TTR_FAIL_LINSOLVE) }
            TRLIB_DNRM2(norm_sol0, &n0, sol0, &inc)
    
            if (*iter_newton % 20 == 1) {
                if (unicode) { TRLIB_PRINTLN_1("%6s%14s%14s%14s", " iter ", "       \u03bb      ", "      d\u03bb      ", " \u2016h\u2080(\u03bb)\u2016-radius") }
                else { TRLIB_PRINTLN_1("%6s%14s%14s%14s", " iter ", "     lam      ", "     dlam     ", "  tr resdidual") }
            }
            TRLIB_PRINTLN_1("%6d%14e%14e%14e", *iter_newton, *lam0, dlam, norm_sol0-radius)
    
            // test for convergence
            if (norm_sol0 - radius <= tol_rel * radius) {
                // what if norm_sol < radius in a significant way?
                // theory tells this should not happen...
                    
                ret = TRLIB_TTR_CONV_BOUND; break;
            }
        }
    }

    *warm0 = 1;

    /* now in a situation were accurate lam0, h_0 exists to first irreducible block
     * invoke Theorem 5.8:
     * (i)  if lam0 >= -leftmost the pair lam0, h_0 solves the problem
     * (ii) if lam0 < -leftmost a solution has to be constructed to lam = -leftmost */

    // quick exit: only one irreducible block
    if (nirblk == 1) {
        *lam = *lam0; *warm = 1;
        TRLIB_DCOPY(&n0, sol0, &inc, sol, &inc) // sol <== sol0
        // compute objective. first store 2*gradient in w, then compute obj = .5*(sol, w)
        TRLIB_DCOPY(&n0, neglin, &inc, w, &inc) ferr = -2.0; TRLIB_DSCAL(&n0, &ferr, w, &inc) ferr = 1.0; // w <-- -2 neglin
        TRLIB_DLAGTM("N", &n0, &inc, &ferr, offdiag, diag, offdiag, sol, &n0, &ferr, w, &n0) // w <-- T*sol + w
        TRLIB_DDOT(dot, &n0, sol, &inc, w, &inc) *obj = 0.5*dot; // obj = .5*(sol, w)
        TRLIB_RETURN(ret)
    }

    // now that we have accurate lam, h_0 invoke Theorem 5.8
    // check if lam <= leftmost --> in that case the first block information describes everything
    TRLIB_PRINTLN_1("\nCheck if \u03bb\u2080 provides global solution, get leftmost ev for irred blocks")
    if(!*warm_leftmost) {
        *sub_fail = trlib_leftmost(nirblk, irblk, diag, offdiag, 0, leftmost[nirblk-1], 1000, TRLIB_EPS_POW_75, verbose-2, unicode, " LM ", fout, timing+10, ileftmost, leftmost);
        *warm_leftmost = 1;
    }
    TRLIB_PRINTLN_1("    leftmost = %e (block %d)", leftmost[*ileftmost], *ileftmost)
    if(*lam0 >= -leftmost[*ileftmost]) {
        if (unicode) { TRLIB_PRINTLN_1("  \u03bb\u2080 \u2265 -leftmost \u21d2 \u03bb = \u03bb\u2080, exit: h\u2080(\u03bb\u2080)") }
        else { TRLIB_PRINTLN_1("  lam0 >= -leftmost => lam = lam0, exit: h0(lam0)") }
        *lam = *lam0; *warm = 1;
        TRLIB_DCOPY(&n0, sol0, &inc, sol, &inc) // sol <== sol0
        // compute objective. first store 2*gradient in w, then compute obj = .5*(sol, w)
        TRLIB_DCOPY(&n0, neglin, &inc, w, &inc) ferr = -2.0; TRLIB_DSCAL(&n0, &ferr, w, &inc) ferr = 1.0; // w <-- -2 neglin
        TRLIB_DLAGTM("N", &n0, &inc, &ferr, offdiag, diag, offdiag, sol, &n0, &ferr, w, &n0) // w <-- T*sol + w
        TRLIB_DDOT(dot, &n0, sol, &inc, w, &inc) *obj = 0.5*dot; // obj = .5*(sol, w)
        TRLIB_RETURN(ret)
    }
    else {
        if (unicode) { TRLIB_PRINTLN_1("  -leftmost > \u03bb\u2080 \u21d2 \u03bb = -leftmost, exit: h\u2080(-leftmost) + \u03b1 u") }
        else  { TRLIB_PRINTLN_1("  -leftmost > lam0 => lam = -leftmost, exit: h0(-leftmost) + alpha u") }

        // Compute solution of (T0 - leftmost*I)*h0 = neglin
        *lam = -leftmost[*ileftmost]; *warm = 1;
        TRLIB_DCOPY(&n0, neglin, &inc, sol, &inc) // neglin <-- sol
        if(!*warm_fac){
            TRLIB_DCOPY(&n0, diag, &inc, diag_lam, &inc) // diag_lam <-- diag
            TRLIB_DAXPY(&n0, lam, ones, &inc, diag_lam, &inc) // diag_lam <-- lam + diag_lam
            TRLIB_DCOPY(&n0, diag_lam, &inc, diag_fac, &inc) // diag_fac <-- diag_lam
            TRLIB_DCOPY(&nm0, offdiag, &inc, offdiag_fac, &inc) // offdiag_fac <-- offdiag
            TRLIB_DPTTRF(&n0, diag_fac, offdiag_fac, &info_fac) // compute factorization
            if (info_fac != 0) { TRLIB_RETURN(TRLIB_TTR_FAIL_FACTOR) } 
        }
        *warm_fac = 1;
        TRLIB_DPTTRS(&n0, &inc, diag_fac, offdiag_fac, sol, &n0, &info_fac) // sol <-- (T+lam I)^-1 sol
        if (info_fac != 0) { TRLIB_RETURN(TRLIB_TTR_FAIL_LINSOLVE) }
        if (refine) { TRLIB_DPTRFS(&n0, &inc, diag_lam, offdiag, diag_fac, offdiag_fac, neglin, &n0, sol, &n0, &ferr, &berr, work, &info_fac) }
        if (info_fac != 0) { TRLIB_RETURN(TRLIB_TTR_FAIL_LINSOLVE) }
        TRLIB_DNRM2(norm_sol0, &n0, sol, &inc)

        // compute normalized eigenvector u corresponding to leftmost of block ileftmost
        srand((unsigned) time(NULL));
        for( int kk = irblk[*ileftmost]; kk < irblk[*ileftmost+1]; ++kk ) { sol[kk] = ((double)rand()/(double)RAND_MAX); }
        nl = irblk[*ileftmost+1]-irblk[*ileftmost];
        *sub_fail = trlib_eigen_inverse(nl, diag+irblk[*ileftmost], offdiag+irblk[*ileftmost], 
                leftmost[*ileftmost], 10, TRLIB_EPS_POW_5, ones,
                diag_fac+irblk[*ileftmost], offdiag_fac+irblk[*ileftmost],
                sol+irblk[*ileftmost], 
                verbose-2, unicode, " EI", NULL, timing+11, &ferr, &berr, &jj); // can savely overwrite ferr, berr, jj with results. only interesting: eigenvector
        if (*sub_fail != 0) { TRLIB_RETURN(TRLIB_TTR_FAIL_EIG) }

        // solution is of form [h,0,...,0,alpha*u,0,...,0]
        // alpha = sqrt( radius^2 - ||h||^2 )
        ferr = sqrt( radius*radius - norm_sol0*norm_sol0 );
        TRLIB_DSCAL(&nl, &ferr, sol+irblk[*ileftmost], &inc)

        if (unicode) { TRLIB_PRINTLN_1("    with \u2016h\u2080(-leftmost)\u2016 = %e, \u03b1 = %e", norm_sol0, ferr) }
        else { TRLIB_PRINTLN_1("    with ||h0(-leftmost)|| = %e, alpha = %e", norm_sol0, ferr) }

        ret = TRLIB_TTR_HARD;
        
        // compute objective. first store 2*gradient in w, then compute obj = .5*(sol, w)
        *obj = 0.5*leftmost[*ileftmost]*ferr*ferr;
        TRLIB_DCOPY(&n0, neglin, &inc, w, &inc) ferr = -2.0; TRLIB_DSCAL(&n0, &ferr, w, &inc) ferr = 1.0; // w <-- -2 neglin
        TRLIB_DLAGTM("N", &n0, &inc, &ferr, offdiag, diag, offdiag, sol, &n0, &ferr, w, &n0) // w <-- T*sol + w
        TRLIB_DDOT(dot, &n0, sol, &inc, w, &inc) *obj = *obj+0.5*dot; // obj = .5*(sol, w)
        TRLIB_RETURN(ret);
    }
}
