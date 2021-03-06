
#include <Rdefines.h>
#include <R_ext/Rdynload.h>
#include <R_ext/Visibility.h>
#include <Rcpp.h>
#include <RcppEigen.h>

#include "oem.h"

using namespace Rcpp;
using namespace RcppEigen;

RcppExport SEXP oem_fit_big(SEXP, 
                            SEXP, 
                            SEXP,
                            SEXP,
                            SEXP,
                            SEXP,
                            SEXP,
                            SEXP,
                            SEXP,
                            SEXP, 
                            SEXP,
                            SEXP,
                            SEXP,
                            SEXP,
                            SEXP, 
                            SEXP,
                            SEXP,
                            SEXP);

RcppExport SEXP oem_fit_dense(SEXP, 
                              SEXP, 
                              SEXP,
                              SEXP,
                              SEXP,
                              SEXP,
                              SEXP,
                              SEXP,
                              SEXP,
                              SEXP, 
                              SEXP,
                              SEXP,
                              SEXP,
                              SEXP,
                              SEXP, 
                              SEXP,
                              SEXP,
                              SEXP);

RcppExport SEXP oem_fit_logistic_dense(SEXP, 
                                       SEXP, 
                                       SEXP,
                                       SEXP,
                                       SEXP,
                                       SEXP,
                                       SEXP,
                                       SEXP,
                                       SEXP,
                                       SEXP, 
                                       SEXP,
                                       SEXP,
                                       SEXP,
                                       SEXP,
                                       SEXP, 
                                       SEXP,
                                       SEXP,
                                       SEXP);

RcppExport SEXP oem_fit_logistic_sparse(SEXP, 
                                        SEXP, 
                                        SEXP,
                                        SEXP,
                                        SEXP,
                                        SEXP,
                                        SEXP,
                                        SEXP,
                                        SEXP,
                                        SEXP, 
                                        SEXP,
                                        SEXP,
                                        SEXP,
                                        SEXP,
                                        SEXP, 
                                        SEXP,
                                        SEXP,
                                        SEXP);

RcppExport SEXP oem_fit_sparse(SEXP, 
                               SEXP, 
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP, 
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP, 
                               SEXP,
                               SEXP,
                               SEXP);

RcppExport SEXP oem_xtx(SEXP, 
                        SEXP, 
                        SEXP,
                        SEXP,
                        SEXP,
                        SEXP,
                        SEXP,
                        SEXP,
                        SEXP, 
                        SEXP,
                        SEXP,
                        SEXP,
                        SEXP,
                        SEXP,
                        SEXP);

RcppExport SEXP oem_xval_dense(SEXP, 
                               SEXP, 
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP, 
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP, 
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP,
                               SEXP);



