# envf

Like printf for your environment.

When passed a filename containing a '.env' suffix it will replace all instances of `%VARIABLE%` found within with their matching environment variable values.

Unmatched environment variables or malformed variable names will be ignored.

Common usage:

    find . -type f -name '*.env' -exec envf {} \; -delete


An example file:

    [icinga_ido]
    type = "db"
    db = "mysql"
    host = "%ICINGA_DBHOST"
    port = "3306"
    dbname = "%ICINGA_DBNAME%"
    username = "%ICINGA_USERNAME%"
    password = "%ICINGA_PASSWORD%"
    charset = "utf8"
    use_ssl = "0"

