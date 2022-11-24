# https://www.tutorialspoint.com/sqlalchemy/index.htm

from pprint import PrettyPrinter
pp = PrettyPrinter(indent=2)
print = pp.pprint



# make a connection string to a specified DB

from urllib import parse
from sqlalchemy import *
from sqlalchemy.orm import *

connection_string = "mysql+mysqldb://{user}{password}@{server}/{database}".format(
    user=parse.quote("rfamro"),
    password=parse.quote(""),
    server="mysql-rfam-public.ebi.ac.uk:4497",
    database="Rfam",
)
print(connection_string)



# make a session that connected to a specified DB

engine = create_engine(connection_string)
print(engine)



# auto generate Python's class models from SQL's data structure for a specified DB

# https://docs.rfam.org/en/latest/database.html
# sqlacodegen mysql+mysqldb://rfamro@mysql-rfam-public.ebi.ac.uk:4497/Rfam --tables clan,author,version --outfile sql_models.py

from sql_models import *



# SQL query with ORM style

# https://docs.sqlalchemy.org/en/20/orm/session_basics.html
# https://docs.sqlalchemy.org/en/20/orm/quickstart.html
# https://www.tutorialspoint.com/sqlalchemy/sqlalchemy_orm_using_query.htm
# https://flask-sqlalchemy.palletsprojects.com/en/2.x/queries/
# https://www.geeksforgeeks.org/querying-and-selecting-specific-column-in-sqlalchemy/

with Session(bind=engine) as session: # Note: A transaction is auto begun as default
    print(session)

    # Get

    # get by query statement string
    result = session.query(Clan).from_statement(text("SELECT clan_acc, id, author, description FROM clan WHERE author='Gardner PP' ORDER BY id ASC LIMIT 5"))
    for e in result.all(): print(vars(e))

    # filter_by
    fields = ["clan_acc", "id", "author", "description"]
    result = session.query(Clan).options(load_only(*fields)).filter_by(author="Gardner PP").order_by(asc(Clan.id)).limit(5)
    for e in result.all(): print(vars(e))

    # filter + startswith + order_by + limit
    result = session.query(Clan).filter(Clan.id.startswith("CRISPR")).order_by(asc(Clan.id)).limit(5)
    print(vars(result.first()))

    # filter + startswith
    result = session.query(Clan).filter(Clan.id.startswith("mir")).limit(5)
    for e in result.all(): print(vars(e))

    # like
    result = session.query(Clan).filter(Clan.id.like("%rna%")).limit(5)
    for e in result.all(): print(vars(e))

    # insert

    try:
        # session.add(Author(name="Vic P.", initials="VP"))
        session.add_all([
            Author(name="Vic P. 1", initials="VP 1"),
            Author(name="Vic P. 2", initials="VP 2"),
            Author(name="Vic P. 3", initials="VP 3"),
        ])
        session.commit()
    except Exception as e:
        session.rollback()
        print(e)

    # update

    try:
        session.query(Author).where(Author.name == "Gardner P").update({
            Author.name: "Vic P.", 
            Author.initials: "VP",
        })
        session.commit()
    except Exception as e:
        session.rollback()
        print(e)

    # delete

    try:
        session.query(Author).where(Author.name == "Gardner P").delete()
        session.commit()
    except Exception as e:
        session.rollback()
        print(e)



# # SQL query with classic style

# # https://docs.sqlalchemy.org/en/20/core/connections.html

# with engine.connect() as connection: # with engine.begin() as connection:
#     print(connection)

#     result = connection.execute(text("SELECT clan_acc, id, author, description FROM clan WHERE author='Gardner PP' ORDER BY id ASC LIMIT 5"))
#     for e in result.all(): print(e)

#     with connection.begin():
#         pass # transaction here
