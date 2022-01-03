library(stringi)
pat = "[A-Z]"
write.table(data.frame(n=stri_rand_strings(100000,12,pat)),"rnames1.csv",row.names = FALSE,col.names = FALSE)
write.table(data.frame(n=stri_rand_strings(100000,12,pat)),"rnames2.csv",row.names = FALSE,col.names = FALSE)
