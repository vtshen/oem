
[![version](http://www.r-pkg.org/badges/version/oem)](https://cran.r-project.org/package=oem)

### Build Status
|  OS             | Build           |
|-----------------|-----------------|
| Linux x86_64 / OSX   | [![Build Status](https://travis-ci.org/jaredhuling/oem.svg?branch=master)](https://travis-ci.org/jaredhuling/oem)      | 
| Windows x86_64     | [![Appveyor Build Status](https://ci.appveyor.com/api/projects/status/github/jaredhuling/oem?branch=master&svg=true)](https://ci.appveyor.com/project/jaredhuling/oem)     |










## Introduction

The oem package provides estimaton for various penalized linear models using the [Orthogonalizing EM algorithm](http://amstat.tandfonline.com/doi/abs/10.1080/00401706.2015.1054436). Documentation for the package can be found here: [oem site](http://casualinference.org/oem) (still under construction).

Install using the **devtools** package (RcppEigen must be installed first as well):


```r
devtools::install_github("jaredhuling/oem")
```

or by cloning and building using `R CMD INSTALL`

## Models

### Lasso


```r
library(microbenchmark)
library(glmnet)
library(oem)
# compute the full solution path, n > p
set.seed(123)
n <- 1000000
p <- 100
m <- 25
b <- matrix(c(runif(m), rep(0, p - m)))
x <- matrix(rnorm(n * p, sd = 3), n, p)
y <- drop(x %*% b) + rnorm(n)

lambdas = oem(x, y, intercept = TRUE, standardize = FALSE, penalty = "elastic.net")$lambda

microbenchmark(
    "glmnet[lasso]" = {res1 <- glmnet(x, y, thresh = 1e-10, 
                                      standardize = FALSE,
                                      intercept = TRUE,
                                      lambda = lambdas)}, 
    "oem[lasso]"    = {res2 <- oem(x, y,
                                   penalty = "elastic.net",
                                   intercept = TRUE, 
                                   standardize = FALSE,
                                   lambda = lambdas,
                                   tol = 1e-10)},
    times = 5
)
```

```
## Unit: seconds
##           expr      min       lq     mean   median       uq      max neval
##  glmnet[lasso] 7.928666 8.292441 8.720699 8.749717 8.869741 9.762932     5
##     oem[lasso] 2.147068 2.276210 2.313844 2.300738 2.332990 2.512210     5
```

```r
# difference of results
max(abs(coef(res1) - res2$beta[[1]]))
```

```
## [1] 1.048072e-07
```

```r
res1 <- glmnet(x, y, thresh = 1e-12, # thresh must be very low for glmnet to be accurate
                                      standardize = FALSE,
                                      intercept = TRUE,
                                      lambda = lambdas)

max(abs(coef(res1) - res2$beta[[1]]))
```

```
## [1] 3.763398e-09
```

### Nonconvex Penalties


```r
library(sparsenet)
library(ncvreg)
library(plus)
# compute the full solution path, n > p
set.seed(123)
n <- 5000
p <- 200
m <- 25
b <- matrix(c(runif(m, -0.5, 0.5), rep(0, p - m)))
x <- matrix(rnorm(n * p, sd = 3), n, p)
y <- drop(x %*% b) + rnorm(n)

mcp.lam <- oem(x, y, penalty = "mcp",
               gamma = 2, intercept = TRUE, 
               standardize = TRUE,
               nlambda = 200, tol = 1e-10)$lambda

scad.lam <- oem(x, y, penalty = "scad",
               gamma = 4, intercept = TRUE, 
               standardize = TRUE,
               nlambda = 200, tol = 1e-10)$lambda

microbenchmark(
    "sparsenet[mcp]" = {res1 <- sparsenet(x, y, thresh = 1e-10, 
                                          gamma = c(2,3), #sparsenet throws an error 
                                                          #if you only fit 1 value of gamma
                                          nlambda = 200)},
    "oem[mcp]"    = {res2 <- oem(x, y,  
                                 penalty = "mcp",
                                 gamma = 2,
                                 intercept = TRUE, 
                                 standardize = TRUE,
                                 nlambda = 200,
                                 tol = 1e-10)},
    "ncvreg[mcp]"    = {res3 <- ncvreg(x, y,  
                                   penalty = "MCP",
                                   gamma = 2,
                                   lambda = mcp.lam,
                                   eps = 1e-7)}, 
    "plus[mcp]"    = {res4 <- plus(x, y,  
                                   method = "mc+",
                                   gamma = 2,
                                   lam = mcp.lam)},
    "oem[scad]"    = {res5 <- oem(x, y,  
                                 penalty = "scad",
                                 gamma = 4,
                                 intercept = TRUE, 
                                 standardize = TRUE,
                                 nlambda = 200,
                                 tol = 1e-10)},
    "ncvreg[scad]"    = {res6 <- ncvreg(x, y,  
                                   penalty = "SCAD",
                                   gamma = 4,
                                   lambda = scad.lam,
                                   eps = 1e-7)}, 
    "plus[scad]"    = {res7 <- plus(x, y,  
                                   method = "scad",
                                   gamma = 4,
                                   lam = scad.lam)}, 
    times = 5
)
```

```
## Warning message: some lam not reached by the plus path and dropped
## Warning message: some lam not reached by the plus path and dropped
## Warning message: some lam not reached by the plus path and dropped
## Warning message: some lam not reached by the plus path and dropped
## Warning message: some lam not reached by the plus path and dropped
## Warning message: some lam not reached by the plus path and dropped
## Warning message: some lam not reached by the plus path and dropped
## Warning message: some lam not reached by the plus path and dropped
## Warning message: some lam not reached by the plus path and dropped
## Warning message: some lam not reached by the plus path and dropped
```

```
## Unit: milliseconds
##            expr       min        lq      mean    median        uq
##  sparsenet[mcp] 1874.3928 1878.6945 2002.3684 2057.3514 2081.0070
##        oem[mcp]  164.1463  166.5305  173.0275  175.5607  177.2974
##     ncvreg[mcp] 7747.7238 7776.5543 7925.6028 7844.6391 7918.3745
##       plus[mcp] 1767.6749 1862.9528 1919.7753 1934.2133 1961.2102
##       oem[scad]  138.0679  139.6930  144.4245  145.4177  146.5624
##    ncvreg[scad] 8168.3490 8190.0219 8255.9754 8196.8696 8299.8637
##      plus[scad] 2006.9863 2065.7875 2219.5906 2098.4239 2376.2346
##        max neval
##  2120.3964     5
##   181.6027     5
##  8340.7222     5
##  2072.8255     5
##   152.3815     5
##  8424.7730     5
##  2550.5207     5
```

```r
diffs <- array(NA, dim = c(4, 1))
colnames(diffs) <- "abs diff"
rownames(diffs) <- c("MCP:  oem and ncvreg", "SCAD: oem and ncvreg",
                     "MCP:  oem and plus", "SCAD: oem and plus")
diffs[,1] <- c(max(abs(res2$beta[[1]] - res3$beta)), max(abs(res5$beta[[1]] - res6$beta)),
               max(abs(res2$beta[[1]][-1,1:nrow(res4$beta)] - t(res4$beta))),
               max(abs(res5$beta[[1]][-1,1:nrow(res7$beta)] - t(res7$beta))))
diffs
```

```
##                          abs diff
## MCP:  oem and ncvreg 1.725859e-07
## SCAD: oem and ncvreg 5.094648e-08
## MCP:  oem and plus   2.684108e-11
## SCAD: oem and plus   1.732414e-11
```



### Group Lasso


```r
library(gglasso)
library(grpreg)
library(grplasso)
# compute the full solution path, n > p
set.seed(123)
n <- 5000
p <- 200
m <- 25
b <- matrix(c(runif(m, -0.5, 0.5), rep(0, p - m)))
x <- matrix(rnorm(n * p, sd = 3), n, p)
y <- drop(x %*% b) + rnorm(n)
groups <- rep(1:floor(p/10), each = 10)

grp.lam <- oem(x, y, penalty = "grp.lasso",
               groups = groups,
               nlambda = 100, tol = 1e-10)$lambda


microbenchmark(
    "gglasso[grp.lasso]" = {res1 <- gglasso(x, y, group = groups, 
                                            lambda = grp.lam, 
                                            intercept = FALSE,
                                            eps = 1e-8)},
    "oem[grp.lasso]"    = {res2 <- oem(x, y,  
                                       groups = groups,
                                       intercept = FALSE,
                                       standardize = FALSE,
                                       penalty = "grp.lasso",
                                       lambda = grp.lam,
                                       tol = 1e-10)},
    "grplasso[grp.lasso]"    = {res3 <- grplasso(x=x, y=y, 
                                                 index = groups,
                                                 standardize = FALSE, 
                                                 center = FALSE, model = LinReg(), 
                                                 lambda = grp.lam * n * 2, 
                                                 control = grpl.control(trace = 0, tol = 1e-10))}, 
    "grpreg[grp.lasso]"    = {res4 <- grpreg(x, y, group = groups, 
                                             eps = 1e-10, lambda = grp.lam)},
    times = 5
)
```

```
## Unit: milliseconds
##                 expr       min         lq       mean     median        uq
##   gglasso[grp.lasso] 1978.9528 2055.48279 2074.99003 2061.55882 2075.8160
##       oem[grp.lasso]   88.9876   89.03062   89.69803   89.04799   89.8331
##  grplasso[grp.lasso] 5118.5293 5179.82449 5346.66026 5195.05384 5581.9713
##    grpreg[grp.lasso] 1222.1581 1262.40725 1338.06540 1314.12198 1340.3891
##         max neval
##  2203.13981     5
##    91.59082     5
##  5657.92228     5
##  1551.25053     5
```

```r
diffs <- array(NA, dim = c(2, 1))
colnames(diffs) <- "abs diff"
rownames(diffs) <- c("oem and gglasso", "oem and grplasso")
diffs[,1] <- c(  max(abs(res2$beta[[1]][-1,] - res1$beta)), max(abs(res2$beta[[1]][-1,] - res3$coefficients))  )
diffs
```

```
##                      abs diff
## oem and gglasso  1.729379e-06
## oem and grplasso 4.828369e-08
```

#### Bigger Group Lasso Example


```r
set.seed(123)
n <- 500000
p <- 200
m <- 25
b <- matrix(c(runif(m, -0.5, 0.5), rep(0, p - m)))
x <- matrix(rnorm(n * p, sd = 3), n, p)
y <- drop(x %*% b) + rnorm(n)
groups <- rep(1:floor(p/10), each = 10)

system.time(res <- oem(x, y, penalty = "grp.lasso",
                       groups = groups,
                       standardize = TRUE,
                       intercept = TRUE,
                       nlambda = 100, tol = 1e-10))
```

```
##    user  system elapsed 
##    3.24    0.20    3.68
```

```r
# memory usage is out of control here.
# oem uses approximately 1/3 of the memory
system.time(res2 <- grpreg(x, y, group = groups, 
                           eps = 1e-10, lambda = res$lambda))
```

```
##    user  system elapsed 
##   72.68    1.64   76.74
```

```r
# I think the standardization is done
# differently for grpreg
max(abs(res$beta[[1]] - res2$beta))
```

```
## [1] 0.0007842304
```

```r
mean(abs(res$beta[[1]] - res2$beta))
```

```
## [1] 8.363514e-06
```

### Fitting Multiple Penalties

The oem algorithm is quite efficient at fitting multiple penalties simultaneously when p is not too big.


```r
set.seed(123)
n <- 100000
p <- 100
m <- 15
b <- matrix(c(runif(m, -0.25, 0.25), rep(0, p - m)))
x <- matrix(rnorm(n * p, sd = 3), n, p)
y <- drop(x %*% b) + rnorm(n)

microbenchmark(
    "oem[lasso]"    = {res1 <- oem(x, y,
                                   penalty = "elastic.net",
                                   intercept = TRUE, 
                                   standardize = TRUE,
                                   tol = 1e-10)},
    "oem[lasso/mcp/scad/ols]"    = {res2 <- oem(x, y,
                                   penalty = c("elastic.net", "mcp", "scad", "grp.lasso"),
                                   gamma = 4,
                                   groups = rep(1:10, each = 10),
                                   intercept = TRUE, 
                                   standardize = TRUE,
                                   tol = 1e-10)},
    times = 5
)
```

```
## Unit: milliseconds
##                     expr      min       lq     mean   median       uq
##               oem[lasso] 226.2586 243.8077 261.7543 247.9962 251.6194
##  oem[lasso/mcp/scad/ols] 255.9322 269.5483 283.0102 272.1617 282.6638
##       max neval
##  339.0896     5
##  334.7453     5
```

```r
#png("../mcp_path.png", width = 3000, height = 3000, res = 400);par(mar=c(5.1,5.1,4.1,2.1));plot(res2, which.model = 2, main = "mcp",lwd = 3,cex.axis=2.0, cex.lab=2.0, cex.main=2.0, cex.sub=2.0);dev.off()
#

layout(matrix(1:4, ncol=2, byrow = TRUE))
plot(res2, which.model = 1, lwd = 2,
     xvar = "lambda")
plot(res2, which.model = 2, lwd = 2,
     xvar = "lambda")
plot(res2, which.model = 3, lwd = 2,
     xvar = "lambda")
plot(res2, which.model = 4, lwd = 2,
     xvar = "lambda")
```

<img src="vignettes/mult-1.png" style="display: block; margin: auto;" />
