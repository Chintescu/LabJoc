
<html>
	
<head>
 <title>Articles</title>
    <style>
    * {
      font-family: arial, sans-serif;
      box-sizing: border-box;
	  font-size : 20px;
	  color : FBF4F0;
    }
    #demo { border-collapse: collapse; }
    #demo td { padding: 10px; }
    </style>
<link rel= "stylesheet" type= "text/css" href= "{{ url_for('static',filename='css/navbar.css') }}">
<link rel= "stylesheet" type= "text/css" href= "{{ url_for('static',filename='css/site.css') }}">
		<div class="topnav">

		<a class="nav-link" href="/">Home </a>
		
		<a class="nav-link" href="/articles">Articles</a>
				
		<a class="nav-link" href="/books">Books</a>
				
		<a class="nav-link" href="/login">Login</a>
		
		<a class="nav-link" href="/wishList">WishList</a>
	</div>
</head>
  <body style=" background-image: url('/static/images/book.jpg')">
<table id="demo">
  <!-- HEADER ROW -->
  <tr>
    {% for col in header %}
    <th>{{ col }}</th>
    {% endfor %}
  </tr>
 
  <!-- DATA ROWS -->
  {% for row in rows %}
  <tr>
    {% for col in row %}
    <td>{{ col }}</td>
    {% endfor %}
  </tr>
  {% endfor %}
</table>
  </body>
</html>