import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Grab all required data for display
    transData = db.execute("SELECT stock, shares FROM transactions WHERE user_id = ? ORDER BY stock ASC", session["user_id"])
    userCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

    # Create required variables
    displayList = []
    stockName = []

    # Iterate through each transaction and decide an action
    for transaction in transData:
        # If stock is new, create entry in displayList
        if transaction["stock"] not in stockName:
            price = lookup(transaction["stock"])
            price = price["price"]
            tempDict = {
                "stock": transaction["stock"],
                "shares": transaction["shares"],
                "price": price,
                "value": (transaction["shares"] * price)
            }
            stockName.append(transaction["stock"])
            displayList.append(tempDict)
        else:
            # If stock exists in portfolio, just update shares and value
            displayList[stockName.index(transaction["stock"])]["shares"] += transaction["shares"]
            displayList[stockName.index(transaction["stock"])]["value"] = price * displayList[stockName.index(transaction["stock"])]["shares"]

    # Calculate total cash in portfolio
    sumMoney = 0

    for cash in displayList:
        sumMoney += cash["value"]

    return render_template("/index.html", displayData=displayList, userCash=userCash[0]["cash"], sumMoney=(userCash[0]["cash"] + sumMoney))


@app.route("/cash",  methods=["GET", "POST"])
@login_required
def cash():

    if request.method == "GET":
        return render_template("cash.html")
    elif request.method == "POST":

        checkInt = True

        # Force user to input positive int
        try:
            money = float(request.form.get("cash"))
        except ValueError:
            checkInt = False

        if checkInt == False or money <= 0:
            return apology("must enter positive integer")

        # Grab cash value from database
        currentCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        db.execute("UPDATE users SET cash = ? WHERE id = ?", (currentCash[0]["cash"] + money), session["user_id"])

        return redirect("/")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # Render buy page template
    if request.method == "GET":
        return render_template("buy.html")
    elif request.method == "POST":
        # Grab stock data
        stockData = lookup(request.form.get("symbol").upper())
        checkInt = True

        # Force user to input positive int
        try:
            shareCount = int(request.form.get("shares"))
        except ValueError:
            checkInt = False

        if not stockData:
            return apology("must provide valid symbol")
        elif checkInt == False or shareCount <= 0:
            return apology("must enter positive integer")

        # Grab cash value from database
        currentCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

        # Check if user can afford
        cost = stockData["price"] * shareCount
        if currentCash[0]["cash"] < cost:
            return apology("insufficient cash for purchase")
        else:
            # Update cash value
            db.execute("UPDATE users SET cash = ? WHERE id = ?", currentCash[0]["cash"] - cost, session["user_id"])

            # Record transaction
            db.execute("INSERT INTO transactions (user_id, stock, price, shares) VALUES (?, ?, ?, ?)", session["user_id"], stockData["symbol"], stockData["price"], shareCount)
            return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Grab transaction data
    transData = db.execute("SELECT stock, price, shares, date, time FROM transactions WHERE user_id = ?", session["user_id"])
    return render_template("history.html", transData=transData)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # Render default quote page
    if request.method == "GET":
        return render_template("quote.html")

    # Render stock info page
    elif request.method == "POST":
        stockVal = lookup(request.form.get("symbol").upper())

        # Check for valid stock symbol
        if not stockVal:
            return apology("must provide valid symbol")
        else:
            return render_template("quoted.html", stockVals=stockVal)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Grab SQL username data and store in variable
    usernames = db.execute("SELECT username FROM users")

    # Grab input forms and assign to variables
    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    if request.method == "POST":
        # Check if username exists in database
        checkUnique = 0
        for users in usernames:
            if users["username"] == username:
                checkUnique = 1

        # Check for unique / non-blank username
        if not username or checkUnique == 1:
            return apology("must provide unique username")

        # Check for blank passwords
        elif not password or not confirmation:
            return apology("must provide password")

        # Check for non-matching passwords
        elif password != confirmation:
            return apology("must provide matching password")

        # Add user and password to database
        else:
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password, method='pbkdf2:sha256', salt_length=8))
            return redirect("/")

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Check which stocks user owns from transaction database
    stocks = db.execute("SELECT DISTINCT stock FROM transactions WHERE user_id = ?", session["user_id"])
    # WHERE user_id = ? AND stock = ?", session["user_id"], stocks[0]["stock"]

    if request.method == "GET":
        return render_template("sell.html", stocks=stocks)
    elif request.method == "POST":

        # Check for proper values
        checkInt = True

        try:
            shareCount = int(request.form.get("shares"))
        except ValueError:
            checkInt = False

        sellStock = request.form.get("symbol")
        stockList = list(map(lambda rec: rec.get("stock"), stocks))
        ownedShares = db.execute("SELECT SUM(shares) FROM transactions WHERE user_id = ? AND stock = ?", session["user_id"], sellStock)
        ownedShares = ownedShares[0]["SUM(shares)"]

        if checkInt == False or shareCount <= 0:
            return apology("must enter positive integer")
        elif not sellStock:
            return apology("must choose stock to sell")
        elif shareCount > ownedShares:
            return apology("insufficient stock owned")
        elif sellStock not in stockList:
            return apology("do not own stock")

        # Sell stock
        stockData = lookup(sellStock)
        currentCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        db.execute("INSERT INTO transactions (user_id, stock, price, shares) VALUES (?, ?, ?, ?)", session["user_id"], sellStock, stockData["price"], -shareCount)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", currentCash[0]["cash"] + (stockData["price"] * shareCount), session["user_id"])
        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
